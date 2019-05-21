import 'package:flutter/material.dart';
import 'package:locksmither/network/cookie_jar.dart';
import 'package:locksmither/models/token.dart';

class HomePage extends StatefulWidget {
   @override
  State<StatefulWidget> createState() {
    return new HomePageState();
  }
}

class HomePageState  extends State<HomePage> {
  Token _token;

  HomePageState() {
    CookieJar jar = new CookieJar();
    _token = jar.getToken();
  }

  String get lockURL => _token.lockURL;
  int get time => _token.time;


  @override
  Widget build(BuildContext context) {
    return new Scaffold(
      appBar: new AppBar(title: new Text("Home"),),
      body: new Center(
        
        child: new Text("Success!\nLock Url: $lockURL\nObtained in: $time nanoseconds"
                ),
              ),
        );
  }

}