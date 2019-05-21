import 'package:flutter/material.dart';
import 'package:locksmither/pages/login_page.dart';
import 'package:locksmither/pages/home_page.dart';

final routes = {
  '/login':         (BuildContext context) => new LoginPage(),
  '/home':         (BuildContext context) => new HomePage(),
  '/' :          (BuildContext context) => new LoginPage(),
};