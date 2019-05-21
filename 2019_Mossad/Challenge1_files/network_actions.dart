import 'dart:async';

import 'dart:convert';
import 'package:locksmither/network/network_wrapper.dart';
import 'package:locksmither/models/token.dart';
import 'package:locksmither/models/AuthURL.dart';

class NetworkActions {
  NetworkWrapper _netUtil = new NetworkWrapper();
  static const BASE_URL = "http://35.246.158.51:8070";
  static const LOGIN_URL = BASE_URL + "/auth/getUrl";

  Future<Token> login(String seed, String password) {
    var headers = new Map<String,String>();
      return _netUtil.get(LOGIN_URL, headers:headers).then((dynamic authUrl) {
      try {
        if (authUrl == null) {
          return Future<Token>.sync(() => new Token("", false, 0));
        }
        var loginUrl = BASE_URL + AuthURL.map(json.decode(authUrl.body)).url;
        Map<String,String> body = { "Seed": seed, "Password": password };
        Map<String,String> headers = {"content-type": "application/json"};
        return _netUtil.post(loginUrl,body: json.encode(body), headers:headers).then((dynamic token) {                
                return Token.map(token);
              });
      } catch (e) {
        return Future<Token>.sync(() => new Token("", false, 0));
      }
      }).catchError((e) { 
        return null; 
      });
  }
}