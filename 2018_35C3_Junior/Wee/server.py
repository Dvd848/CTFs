#!/usr/bin/env python3
import inspect
import os
import random
import sqlite3
import string
import sys
import base64
from html import escape
from urllib import parse
from typing import Union, List, Tuple
import datetime

from subprocess import STDOUT, check_output

import requests
from flask import Flask, send_from_directory, send_file, request, Response, g, make_response, jsonify
from flags import DB_SECRET, DECRYPTED, DEV_NULL, LOCALHOST, LOGGED_IN

STATIC_PATH = "../client/site"
DATABASE = ".paperbots.db"
MIGRATION_PATH = "./db/V1__Create_tables.sql"
THUMBNAIL_PATH = os.path.join(STATIC_PATH, "thumbnails")
WEE_PATH = "../weelang"
WEETERPRETER = "weeterpreter.ts"
WEE_TIMEOUT = 5

os.makedirs(THUMBNAIL_PATH, exist_ok=True)

app = Flask(__name__, static_folder=STATIC_PATH, static_url_path="/static")

encrypted = None


def get_db() -> sqlite3.Connection:
    db = getattr(g, '_database', None)
    if db is None:
        db = g._database = sqlite3.connect(DATABASE)
    return db


def init_db():
    with app.app_context():
        db = get_db()
        with open(MIGRATION_PATH, "r") as f:
            db.cursor().executescript(f.read())
        db.execute("CREATE TABLE `secrets`(`id` INTEGER PRIMARY KEY AUTOINCREMENT, `secret` varchar(255) NOT NULL)")
        db.execute("INSERT INTO secrets(secret) values(?)", (DB_SECRET,))
        db.commit()


def query_db(query, args=(), one=True) -> Union[List[Tuple], Tuple, None]:
    if not isinstance(args, tuple):
        args = (args,)
    cur = get_db().execute(query, args)
    rv = cur.fetchall()
    cur.close()
    return (rv[0] if rv else None) if one else rv


def user_by_token(token) -> Tuple[int, str, str, str]:
    """
    queries and returns userId, username, email, usertype for a given token
    :param token: the token
    :return: userId, name, email, usertype
    """
    if not token:
        raise AttributeError("Token must not be empty")

    userId, = query_db("SELECT userId FROM userTokens WHERE token=?", token)  # TODO: Join this?
    name, email, usertype = query_db("SELECT name, email, type FROM users WHERE id=?", userId)
    return userId, name, email, usertype


def random_code(length=6) -> str:
    return "".join([random.choice(string.ascii_lowercase)[0] for x in range(length)])


def get_code(username):
    db = get_db()
    c = db.cursor()
    userId, = query_db("SELECT id FROM users WHERE name=?", username)
    code = random_code()
    c.execute("INSERT INTO userCodes(userId, code) VALUES(?, ?)", (userId, code))
    db.commit()
    # TODO: Send the code as E-Mail instead :)
    return code


def jsonify_projects(projects, username, usertype):
    return jsonify([
        {"code": x[0],
         "userName": x[1],
         "title": x[2],
         "public": x[3],
         "type": x[4],
         "lastModified": x[5],
         "created": x[6],
         "content": x[7]
         } for x in projects if usertype == "admin" or x[1] == username or x[3]
    ])


@app.teardown_appcontext
def close_connection(exception):
    db = getattr(g, '_database', None)
    if db is not None:
        db.close()


# Error handling
@app.errorhandler(404)
def fourohfour(e):
    return send_file(os.path.join(STATIC_PATH, "404.html")), 404


@app.errorhandler(500)
def fivehundred(e):
    return jsonify({"error": str(e)}), 500


@app.after_request
def secure(response: Response):
    if not request.path[-3:] in ["jpg", "png", "gif"]:
        response.headers["X-Frame-Options"] = "SAMEORIGIN"
        response.headers["X-Xss-Protection"] = "1; mode=block"
        response.headers["X-Content-Type-Options"] = "nosniff"
        response.headers["Content-Security-Policy"] = "script-src 'self' 'unsafe-inline';"
        response.headers["Referrer-Policy"] = "no-referrer-when-downgrade"
        response.headers["Feature-Policy"] = "geolocation 'self'; midi 'self'; sync-xhr 'self'; microphone 'self'; " \
                                             "camera 'self'; magnetometer 'self'; gyroscope 'self'; speaker 'self'; " \
                                             "fullscreen *; payment 'self'; "
        if request.remote_addr == "127.0.0.1":
            response.headers["X-Localhost-Token"] = LOCALHOST

    return response


@app.route("/", methods=["GET"])
def main():
    return send_file(os.path.join(STATIC_PATH, "index.html"))


@app.route("/kitten.png")
def kitten():
    return send_file(os.path.join(STATIC_PATH, "img/kitten.png"))


# The actual page
@app.route("/<path:filename>", methods=["GET"])
def papercontents(filename):
    return send_from_directory(STATIC_PATH, filename)


@app.route("/api/signup", methods=["POST"])
def signup():
    usertype = "user"
    json = request.get_json(force=True)
    name = escape(json["name"].strip())
    email = json["email"].strip()
    if len(name) == 0:
        raise Exception("InvalidUserName")
    if len(email) == 0:
        raise Exception("InvalidEmailAddress")
    if not len(email.split("@")) == 2:
        raise Exception("InvalidEmailAddress")
    email = escape(email.strip())
    # Make sure the user name is 4-25 letters/digits only.
    if len(name) < 4 or len(name) > 25:
        raise Exception("InvalidUserName")

    if not all([x in string.ascii_letters or x in string.digits for x in name]):
        raise Exception("InvalidUserName")
    # Check if name exists
    if query_db("SELECT name FROM users WHERE name=?", name):
        raise Exception("UserExists")
    if query_db("Select id, name FROM users WHERE email=?", email):
        raise Exception("EmailExists")
    # Insert user // TODO: implement the verification email
    db = get_db()
    c = db.cursor()
    c.execute("INSERT INTO users(name, email, type) values(?, ?, ?)", (name, email, usertype))
    db.commit()
    return jsonify({"success": True})


@app.route("/api/login", methods=["POST"])
def login():
    print("Logging in?")
    # TODO Send Mail
    json = request.get_json(force=True)
    login = json["email"].strip()
    try:
        userid, name, email = query_db("SELECT id, name, email FROM users WHERE email=? OR name=?", (login, login))
    except Exception as ex:
        raise Exception("UserDoesNotExist")
    return get_code(name)


@app.route("/api/verify", methods=["POST"])
def verify():
    code = request.get_json(force=True)["code"].strip()
    if not code:
        raise Exception("CouldNotVerifyCode")
    userid, = query_db("SELECT userId FROM userCodes WHERE code=?", code)
    db = get_db()
    c = db.cursor()
    c.execute("DELETE FROM userCodes WHERE userId=?", (userid,))
    token = random_code(32)
    c.execute("INSERT INTO userTokens (userId, token) values(?,?)", (userid, token))
    db.commit()
    name, = query_db("SELECT name FROM users WHERE id=?", (userid,))
    resp = make_response()
    resp.set_cookie("token", token, max_age=2 ** 31 - 1)
    resp.set_cookie("name", name, max_age=2 ** 31 - 1)
    resp.set_cookie("logged_in", LOGGED_IN)
    return resp


@app.route("/api/logout", methods=["POST"])
def logout():
    request.cookies.get("token")
    resp = make_response()
    resp.set_cookie("token", "")
    resp.set_cookie("name", "")
    resp.set_cookie("logged_in", "")
    return resp


@app.route("/api/getproject", methods=["POST"])
def getproject():
    # TODO: Do.
    project_id = request.get_json(force=True)["projectId"]
    token = request.cookies.get("token")
    try:
        userId, name, email, usertype = user_by_token(token)
    except AttributeError:
        name = ""
        usertype = "user"
    project = query_db("SELECT code, userName, title, description, content, public, type, lastModified, created "
                       "FROM projects WHERE code=?", (project_id,))
    if not project or (not project[5] and not name == project[1] and not usertype == "admin"):
        raise Exception("ProjectDoesNotExist")
    return jsonify({
        "code": project[0],
        "userName": project[1],
        "title": project[2],
        "description": project[3],
        "content": project[4],
        "public": project[5],
        "type": project[6],
        "lastModified": project[7],
        "created": project[8]
    })


@app.route("/api/getprojects", methods=["POST"])
def getuserprojects():
    username = request.get_json(force=True)["userName"]
    projects = query_db("SELECT code, userName, title, public, type, lastModified, created, content "
                        "FROM projects WHERE userName=? ORDER BY lastModified DESC", (username), False)
    name = ""
    usertype = "user"
    if "token" in request.cookies:
        userId, name, email, usertype = user_by_token(request.cookies["token"])
    return jsonify_projects(projects, name, usertype)


@app.route("/api/saveproject", methods=["POST"])
def saveproject():
    json = request.get_json(force=True)
    name = request.cookies["name"]
    token = request.cookies["token"]
    # TODO String projectId = paperbots.saveProject(ctx.cookie("token"), request.getCode(), request.getTitle(), request.getDescription(), request.getContent(), request.isPublic(), request.getType());
    userId, username, email, usertype = user_by_token(token)

    db = get_db()
    c = db.cursor()

    if not json["code"]:
        project_id = random_code(6)

        c.execute(
            "INSERT INTO projects(userId, userName, code, title, description, content, public, type) "
            "VALUES(?,?,?,?,?,?,?,?)",
            (userId, username, project_id,
             escape(json["title"]), escape(json["description"]), json["content"], True, json["type"]))
        db.commit()
        return jsonify({"projectId": project_id})
    else:
        c.execute("UPDATE projects SET title=?, description=?, content=?, public=? WHERE code=? AND userId=?",
                  (escape(json["title"]), escape(json["description"]), json["content"], True, json["code"],
                   userId)
                  )
        db.commit()
        return jsonify({"projectId": json["code"]})


@app.route("/api/savethumbnail", methods=["POST"])
def savethumbnail():
    name = request.cookies["name"]
    token = request.cookies["token"]
    userId, username, email, usertype = user_by_token(token)

    json = request.get_json(force=True)
    thumbnail = json["thumbnail"]  # type: str
    project_id = json["projectId"]
    if not thumbnail.startswith("data:image/png;base64,"):
        raise Exception("Hacker")
    thumbnail = thumbnail[len("data:image/png;base64,"):].encode("ascii")
    decoded = base64.b64decode(thumbnail)
    project_username, = query_db("SELECT userName FROM projects WHERE code=?", (project_id,))
    if project_username != username:
        raise Exception("Hack on WeeLang, not the Server!")
    with open(os.path.join(THUMBNAIL_PATH, "{}.png".format(project_id)), "wb+") as f:
        f.write(decoded)
    return jsonify({"projectId": project_id})


@app.route("/api/deleteproject", methods=["POST"])
def deleteproject():
    name = request.cookies["name"]
    token = request.cookies["token"]
    userid, username, email, usertype = user_by_token(token)
    json = request.get_json(force=True)
    projectid = json["projectId"]
    project_username = query_db("SELECT userName FROM projects WHERE code=?", (projectid,))
    if project_username != username and usertype != "admin":
        raise Exception("Nope")
    db = get_db()
    c = db.cursor()
    c.execute("DELETE FROM projects WHERE code=?", (projectid,))
    db.commit()
    # raise Exception("The Internet Never Forgets")
    return {"projectId": projectid}


# Admin endpoints
@app.route("/api/getprojectsadmin", methods=["POST"])
def getprojectsadmin():
    # ProjectsRequest request = ctx.bodyAsClass(ProjectsRequest.class);
    # ctx.json(paperbots.getProjectsAdmin(ctx.cookie("token"), request.sorting, request.dateOffset));
    name = request.cookies["name"]
    token = request.cookies["token"]
    user, username, email, usertype = user_by_token(token)

    json = request.get_json(force=True)
    offset = json["offset"]
    sorting = json["sorting"]

    if name != "admin":
        raise Exception("InvalidUserName")

    sortings = {
        "newest": "created DESC",
        "oldest": "created ASC",
        "lastmodified": "lastModified DESC"
    }
    sql_sorting = sortings[sorting]

    if not offset:
        offset = datetime.datetime.now()

    return jsonify_projects(query_db(
        "SELECT code, userName, title, public, type, lastModified, created, content FROM projects WHERE created < '{}' "
        "ORDER BY {} LIMIT 10".format(offset, sql_sorting), one=False), username, "admin")


@app.route("/api/getfeaturedprojects", methods=["POST"])
def getfeaturedprojects():
    try:
        name = request.cookies["name"]
        token = request.cookies["token"]
        userid, username, email, usertype = user_by_token(token)
    except Exception as ex:
        username = ""
        usertype = "user"

    projects = query_db("SELECT code, userName, title, type, lastModified, created, content FROM projects "
                        "WHERE featured=1 AND public=1 ORDER BY lastModified DESC", one=False)
    return jsonify_projects(projects, username, usertype)


# Proxy images to avoid tainted canvases when thumbnailing.
@app.route("/api/proxyimage", methods=["GET"])
def proxyimage():
    url = request.args.get("url", '')
    parsed = parse.urlparse(url, "http")  # type: parse.ParseResult
    if not parsed.netloc:
        parsed = parsed._replace(netloc=request.host)  # type: parse.ParseResult
    url = parsed.geturl()

    resp = requests.get(url)
    if not resp.headers["Content-Type"].startswith("image/"):
        raise Exception("Not a valid image")

    # See https://stackoverflow.com/a/36601467/1345238
    excluded_headers = ['content-encoding', 'content-length', 'transfer-encoding', 'connection']
    headers = [(name, value) for (name, value) in resp.raw.headers.items()
               if name.lower() not in excluded_headers]

    response = Response(resp.content, resp.status_code, headers)
    return response


# Additional pyserver functions:

# Wee as a service.
def runwee(wee: string) -> string:
    print("{}: running {}".format(request.remote_addr, wee))
    result = check_output(
        ["ts-node", '--cacheDirectory', os.path.join(WEE_PATH, "__cache__"),
         os.path.join(WEE_PATH, WEETERPRETER), wee], shell=False, stderr=STDOUT, timeout=WEE_TIMEOUT,
        cwd=WEE_PATH).decode("utf-8")
    print("{}: result: {}".format(request.remote_addr, result))
    return result


@app.route("/wee/run", methods=["POST"])
def weeservice():
    json = request.get_json(force=True)
    wee = json["code"]
    out = runwee(wee)
    return jsonify({"code": wee, "result": out})


@app.route("/wee/dev/null", methods=["POST"])
def dev_null():
    json = request.get_json(force=True)
    wee = json["code"]
    wee = """
    var DEV_NULL: string = '{}'
    {}
    """.format(DEV_NULL, wee)
    _ = runwee(wee)
    return "GONE"


@app.route("/wee/encryptiontest", methods=["GET"])
def encryptiontest():
    global encrypted
    if not encrypted:
        wee = """
        # we use weelang to encrypt secrets completely secret

        record pubkey
            n: string
            e: string
        end

        var key = pubkey('951477056381671188036079180681828396446164466568923964269373812360568216940258578681673755725586138473475522188240856850626984093905399964041687626629414562063470963902807801143023140969208234239276778397171817582591827008690056789763534174119863046106813515750863733543758319811194784246845138921495556311458180478538856550842509692686396679117903040148607642710832573838027274004952072516749168425434697690016707327002989407014753735313730653189661541750880855213165937564578292464379167857778759136474173425831340306919705672933486711939333953750637729967455118475408369751602538202818190663939706886093046526104043062374288648189070207772477271879494000411582080352364098957455090381238978031676375437980396931371164061080967754225429135119036489128165414029872153856547376448552882344531325480944511714482341088742350110097372766748364926941000441524157824859511557342673524388056049358362600925172299990719998873868038194555465008036497932945812845340638853399732721987228486858193979073913761760370769609347622795498987306822413134236749607735657967667902966667996797241364688793919066445360547749193845825298342626288990158730149727398354192053692360716383851051271618559075048012800235250387837052573541157845958948856954035758915157871993646182544696043757263004887914724250286341123038686355398997399922927237477691269351791943572679717263938613148630387793458838416117454016370454288153779764863162055098229903413503857354581027436855574871814478747237999617879024407403954905986969721336803258774514397600947175650242674193496614652267158753817350136305620268076457813070726099248681642612063203170442453405051455877524709366973062774037044772079720703743828695351198984334830532193564525916901461725538418714517302390850049543856542699391339075976843028654004552169277571339017161697013373622770115406681080294994790626557117129820457988045974009530185622113951540819939983153190486345031549722007896699102268137425607039925174692583738394816628508716999668221820730737934785438568198334912127263127241407430459511422030656861043544813130287622862247904749760983465608684778389799703770877931875268858524702991767450720773677639856979930404508755100624844341829896497906824520180051038779126563860453039035779455387733056343833776802716194138072528278142786901904343407377649000988142255369860324110311816186668720584468851089864315465497405748709976389375632079690963423708940060402561050963276766635011726613211018206198125893007608417148033891841809', '3')

        fun encrypt(message: string, key: pubkey): string
            return bigModPow(strToBig(message), key.e, key.n)
        end

        fun get_cypher(key: pubkey): string
            var message = '{}'
            return encrypt(message, key)
        end
            
        alert(get_cypher(key))
        """.format(DECRYPTED)
        encrypted = runwee(wee)
    return jsonify({"enc": encrypted})


# The pyserver is almost 100% open source!
# Just enough to barely get it running but never to its full potential.
# We got very positive feedback on HN and nobody bothered to run it anyway.
# 11/10 would open source again.
@app.route("/pyserver/server.py", methods=["GET"])
def server_source():
    return Response(inspect.getsource(sys.modules[__name__]), mimetype='text/x-python')


@app.route("/pyserver/flags.py", methods=["GET"])
def server_flags():
    return Response("""
DB_SECRET = "35C3_???"
DECRYPTED = "35C3_???"
DEV_NULL =  "35C3_???"
LOCALHOST = "35C3_???"
LOGGED_IN = "35C3_???"
NOT_IMPLEMENTED = "35C3_???"
    """, mimetype='text/x-python')


@app.route("/weelang/{}".format(WEETERPRETER), methods=["GET"])
def weeterpreter_source():
    return send_file(os.path.join(WEE_PATH, WEETERPRETER), mimetype="text/x-typescript")


@app.route("/weelang/package.json", methods=["GET"])
def weeterpreter_deps():
    return send_file(os.path.join(WEE_PATH, "package.json"))


@app.route("/weelang/flags.ts", methods=["GET"])
def weeterpreter_flags():
    return Response("""
export const CONVERSION_ERROR = "35C3_???"
export const EQUALITY_ERROR = "35C3_???"
export const LAYERS = "35C3_???"
export const NUMBER_ERROR = "35C3_???"
export const WEE_R_LEET = "35C3_???"
export const WEE_TOKEN = "35C3_???"
    """, mimetype="text/x-typescript")


@app.before_first_request
def maybe_init_db():
    if not os.path.exists(DATABASE):
        init_db()


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8075)
