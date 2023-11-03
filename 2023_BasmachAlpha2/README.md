
# Bashmach Alpha New Year Challenge - חידת בסמ"ח אלפא לראש השנה

## Challenge

Basmach Alpha released an easy challenge for New Year on [Instagram](https://www.instagram.com/p/CxI2bR6qKjH/) 
/ [Tiktok](https://www.tiktok.com/@bsmch.alpha/video/7278346784903105794).

Scanning through the video, the first thing we can spot is a URL in the background of one of the frames:

![](images/img1.png)

However, this URL leads to a [silly meme about AI](https://ibb.co/JnDhkXk). A few seconds later,
we get a better lead:

![](images/img2.png)

There's a QR code on the soldier's cup, leading us to the [actual riddle](https://www.idf.il/%D7%90%D7%AA%D7%A8%D7%99-%D7%99%D7%97%D7%99%D7%93%D7%95%D7%AA/%D7%90%D7%92%D7%A3-%D7%94%D7%AA%D7%A7%D7%A9%D7%95%D7%91-%D7%95%D7%94%D7%94%D7%92%D7%A0%D7%94-%D7%91%D7%A1%D7%99%D7%99%D7%91%D7%A8/%D7%9B%D7%9C-%D7%94%D7%9B%D7%AA%D7%91%D7%95%D7%AA/2023/%D7%97%D7%99%D7%93%D7%AA-%D7%AA%D7%A7%D7%A9%D7%95%D7%91-%D7%91%D7%A1%D7%9E%D7%97-%D7%90%D7%9C%D7%A4%D7%90-%D7%91%D7%A1%D7%9E-%D7%97-%D7%91%D7%99%D7%AA-%D7%94%D7%A1%D7%A4%D7%A8-%D7%9C%D7%9E%D7%A7%D7%A6%D7%95%D7%A2%D7%95%D7%AA-%D7%94%D7%9E%D7%97%D7%A9%D7%91-%D7%97%D7%99%D7%93%D7%94-%D7%A1%D7%99%D7%99%D7%91%D7%A8-%D7%90%D7%99%D7%A0%D7%98%D7%9C%D7%99%D7%92%D7%A0%D7%A6%D7%99%D7%94-%D7%9E%D7%9C%D7%90%D7%9B%D7%95%D7%AA%D7%99%D7%AA-ai-%D7%90%D7%92%D7%A3-%D7%94%D7%AA%D7%A7%D7%A9%D7%95%D7%91-%D7%95%D7%94%D7%94%D7%92%D7%A0%D7%94-%D7%91%D7%A1%D7%99%D7%99%D7%91%D7%A8-%D7%91%D7%99%D7%A0%D7%94-%D7%9E%D7%9C%D7%90%D7%9B%D7%95%D7%AA%D7%99%D7%AA-%D7%91%D7%95%D7%98/):

```
אתם סוכנים חשאיים שנשלחו למשימה חשאית - להפיל ארגון סודי וזדוני שמתכנן כבר שנים את השמדת העולם.
לאחר מאמצים, אנשינו מצאו קישור חלקי לאתר הארגון,
אבל לא הצליחו לעבור את שומר הסף החמקמק - בוט שיחה תמים למראה שמופעל ע"י בינה מלאכותית, וברשותו סוד;
רק אם תצליחו להשיג מהבוט את הסוד (באנגלית), תוכלו להתקדם ולהגיע לאתר - ולהציל את העולם.
לשימושכם, הקישור החלקי לאתר הארגון:
_________/https://ai-riddle.bsmch.idf.il
```

There's a link to a basic [AI Bot](https://ai-riddle.bsmch.idf.il/) which we need to 
trick into giving us the secret. Fortunately, the first prompt from [here](https://www.khalidalnajjar.com/capture-the-flag-of-chatgpt-through-prompt-injection/)
does the trick:

![](images/prompt.png)

We visit `https://ai-riddle.bsmch.idf.il/p0rt4l/` and get a login page. At the bottom of the page
we see a long string: `dGhlIHVzZXJuYW1lIGlzICJvbGl2ZXIiIGFuZCB0aGUgcGFzc3dvcmQgaXMgImlsb3ZlbXljYXQxMjMi`.

If we decode is using base64, we get: `base64: the username is "oliver" and the password is "ilovemycat123"`.

We login using those credentials, navigate to "Good thing" and are requested to click a "reCAPTCHA"-like
checkbox claiming "I'm a robot". This gives us an image containing the following text:

```
This is a long text about a long text that's,
well, really long. It's not just about lots of
words on a page, but about how those
words go on and on, like a winding road that
never seems to end. Imagine a never-ending
story where sentences and paragraphs just
keep going, like this one friend at a party
who can't stop talking. So, get ready for a
wild ride through this wordy wilderness,
where the goal isn't just reaching the finish
line, but enjoying the wordy journey itself.
```

We need to enter the text into a text box. A basic OCR can extract the text from the image,
which after being accepted displays a large button to "destroy the orgenization". We click
it and finish the challnge:

![](images/img3.png)

## Links

 * [Official solution](https://www.idf.il/%D7%90%D7%AA%D7%A8%D7%99-%D7%99%D7%97%D7%99%D7%93%D7%95%D7%AA/%D7%90%D7%92%D7%A3-%D7%94%D7%AA%D7%A7%D7%A9%D7%95%D7%91-%D7%95%D7%94%D7%94%D7%92%D7%A0%D7%94-%D7%91%D7%A1%D7%99%D7%99%D7%91%D7%A8/%D7%9B%D7%9C-%D7%94%D7%9B%D7%AA%D7%91%D7%95%D7%AA/2023/%D7%97%D7%99%D7%93%D7%AA-%D7%91%D7%A1%D7%9E%D7%97-%D7%A8%D7%90%D7%A9-%D7%94%D7%A9%D7%A0%D7%94-%D7%A4%D7%AA%D7%A8%D7%95%D7%9F-%D7%97%D7%99%D7%93%D7%AA-%D7%AA%D7%A7%D7%A9%D7%95%D7%91-%D7%90%D7%92%D7%A3-%D7%94%D7%AA%D7%A7%D7%A9%D7%95%D7%91-%D7%95%D7%94%D7%94%D7%92%D7%A0%D7%94-%D7%91%D7%A1%D7%99%D7%99%D7%91%D7%A8-%D7%97%D7%99%D7%93%D7%94-%D7%97%D7%99%D7%93%D7%AA-ai-%D7%91%D7%95%D7%98-%D7%96%D7%99%D7%91%D7%95%D7%98-%D7%90%D7%99%D7%9A-%D7%A4%D7%95%D7%AA%D7%A8%D7%99%D7%9D-%D7%A9%D7%9C%D7%91-%D7%90%D7%97%D7%A8-%D7%A9%D7%9C%D7%91/)