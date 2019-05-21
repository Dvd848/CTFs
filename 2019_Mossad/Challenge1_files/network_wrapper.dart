import 'dart:async';
import 'dart:convert';
import 'package:http/http.dart' as http;

class NetworkWrapper {
  // next three lines makes this class a Singleton
  static const USER_AGENT = "iWalk-v2";
  static NetworkWrapper _instance = new NetworkWrapper.internal();
  NetworkWrapper.internal();
  factory NetworkWrapper() => _instance;

  final JsonDecoder _decoder = new JsonDecoder();

  void addUserAgent(Map headers) {
    if (!headers.containsKey("User-Agent")) {
      headers["User-Agent"] = USER_AGENT;
    }
  }

  Future<dynamic> getObject(String url, {Map headers}) {
    addUserAgent(headers);
    try {
      return http.get(url, headers: headers).then((http.Response response) {
        final String res = response.body;
        final int statusCode = response.statusCode;

        if (statusCode < 200 || statusCode > 400 || json == null) {
          return null;
        }
        return _decoder.convert(res);
      }).catchError((e) { return null; });
    } catch (e) {
      return null;
    }
  }
  
  Future<http.Response> get(String url, {Map<String,String> headers}) {
    addUserAgent(headers);
    try {
      return http.get(url, headers: headers).then((http.Response response) {
        return response;
      }).catchError((e) { return null; });
    } catch (e) {
      return null;
    }
  }

  Future<dynamic> post(String url, {Map<String,String> headers, body, encoding}) {
    addUserAgent(headers);
    try {
      return http
          .post(url, body: body, headers: headers, encoding: encoding)
          .then((http.Response response) {
            final String res = response.body;
            final int statusCode = response.statusCode;

            if (statusCode < 200 || statusCode > 400 || json == null) {
              return null;
            }
            return _decoder.convert(res);
          }).catchError((e) { return null; });
    } catch (e) {
      return null;
    }
  }
}