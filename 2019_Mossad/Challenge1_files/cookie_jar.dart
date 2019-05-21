import 'package:locksmither/models/token.dart';

class CookieJar {
  static CookieJar _instance = new CookieJar.internal();
  CookieJar.internal();
  factory CookieJar() => _instance;

  Token _token;

  void setToken(Token token) {
    this._token = token;
  }

  Token getToken() {
    return this._token;
  }
}