class AuthURL {
  String _url;
  
  AuthURL(this._url);

  AuthURL.map(dynamic obj) {
    this._url = obj["AuthURL"];
  }

  String get url => _url;

  Map<String, dynamic> toMap() {
    var map = new Map<String, dynamic>();
    map["Url"] = _url;

    return map;
  }
}