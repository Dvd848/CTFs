class Token {
  String _lockURL;
  bool _isValid;
  int  _time;
  
  Token(this._lockURL, this._isValid, this._time);

  Token.map(dynamic obj) {
    this._lockURL = obj["LockURL"];
    this._isValid = obj["IsValid"];
    this._time = obj["Time"];
  }

  String get lockURL => _lockURL;
  bool get isValid => _isValid;
  int get time => _time != null ? _time : 0;

  Map<String, dynamic> toMap() {
    var map = new Map<String, dynamic>();
    map["LockURL"] = _lockURL;
    map["IsValid"] = _isValid;
    map["Time"] = _time;

    return map;
  }
}