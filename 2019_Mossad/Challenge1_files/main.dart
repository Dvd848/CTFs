import 'package:flutter/material.dart';
import 'package:locksmither/routes.dart';

void main() => runApp(LockSmitherApp());

class LockSmitherApp extends StatelessWidget {
  @override

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return new MaterialApp(
      title: 'iWalk-LockSmither App',
      theme: new ThemeData(
        primarySwatch: Colors.blue,
      ),
      routes: routes,
    );
  }
}