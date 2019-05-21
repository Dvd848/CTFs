import 'dart:ui';
import 'package:flutter/material.dart';
import 'package:locksmither/network/network_actions.dart';
import 'package:locksmither/network/cookie_jar.dart';
import 'package:locksmither/models/token.dart';

class LoginPage extends StatefulWidget {
  @override
  State<StatefulWidget> createState() {
    return new LoginPageState();
  }
}

class LoginPageState extends State<LoginPage> {
  BuildContext _ctx;

  bool _isLoading = false;
  final formKey = new GlobalKey<FormState>();
  final scaffoldKey = new GlobalKey<ScaffoldState>();
  String _password;
  String _seed;

  NetworkActions _networkActions = new NetworkActions();

  LoginPageState();

  void _submit() async {
    final form = formKey.currentState;

    if (form.validate()) {
      setState(() => _isLoading = true);
      form.save();

      _networkActions.login(_seed, _password)
                      .then((result) => _loginCompleted(result))
                      .catchError((e) { 
                        _loginCompleted(new Token("", false, 0));
                      });
    }
  }

  void _loginCompleted(Token result) async {
    Duration dur = new Duration(microseconds: (result.time/1000).round());
    _showSnackBar("Completed in: " +  dur.inMilliseconds.toString() + " milliseconds result is: " + result.isValid.toString());
    setState(() => _isLoading = false);
    if (result != null && result.isValid) {
      CookieJar jar = new CookieJar();
      jar.setToken(result);
      Navigator.of(_ctx).pushReplacementNamed("/home");
    } 
  }


  void _showSnackBar(String text) {
    scaffoldKey.currentState
        .showSnackBar(new SnackBar(content: new Text(text)));
  }

  @override
  Widget build(BuildContext context) {
    _ctx = context;
    var loginBtn = new RaisedButton(
      onPressed: _submit,
      child: new Text("LOGIN"),
      color: Colors.blue,
    );
    var loginForm = new Column(
      children: <Widget>[
        new Text(
          "iWalk-LockSmither App",
          textScaleFactor: 2.0,
        ),
        new Form(
          key: formKey,
          child: new Column(
            children: <Widget>[
              new Padding(
                padding: const EdgeInsets.all(8.0),
                child: new TextFormField(
                  onSaved: (val) => _seed = val,
                  decoration: new InputDecoration(labelText: "Seed"),
                ),
              ),
              new Padding(
                padding: const EdgeInsets.all(8.0),
                child: new TextFormField(
                  onSaved: (val) => _password = val,
                  decoration: new InputDecoration(labelText: "Password"),
                ),
              ),
            ],
          ),
        ),
        _isLoading ? new CircularProgressIndicator() : loginBtn
      ],
      crossAxisAlignment: CrossAxisAlignment.center,
    );

    return new Scaffold(
      appBar: null,
      key: scaffoldKey,
      body: new Container(
        child: new Center(
          child: new ClipRect(
            child: new BackdropFilter(
              filter: new ImageFilter.blur(sigmaX: 10.0, sigmaY: 10.0),
              child: new Container(
                child: loginForm,
                height: 300.0,
                width: 300.0,
                decoration: new BoxDecoration(
                    color: Colors.grey.shade200.withOpacity(0.5)),
              ),
            ),
          ),
        ),
      ),
    );
  }
}