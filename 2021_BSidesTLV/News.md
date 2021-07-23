# News
* Category: Web
* 150 Points
* Solved by the JCTF Team

## Description

> Keep updated with our new blog, maybe you'll find interesting articles :)

## Solution

We visit the attached website, only to find a standard news portal with many articles. They have "Politics", "Sports", "Art", "Real Estate" - but where is the flag?

Nothing is out of the ordinary in the site. No dynamic forms, no `robots.txt`, nothing in the source code. 

Eventually, we go back to the description: "*Keep updated with our new blog*" it says. How can we do that?

Usually such sites provide an RSS feed under `/rss` or `/feed` (with and without an `.xml` extension). Trying the different possibilities, we finally find:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/News]
└─$ curl -s https://news.ctf.bsidestlv.com/rss.xml
<?xml version="1.0" encoding="UTF-8"?><rss xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:content="http://purl.org/rss/1.0/modules/content/" xmlns:atom="http://www.w3.org/2005/Atom" version="2.0" xmlns:image="http://web.resource.org/rss/1.0/modules/image/">
    <channel>
        <title><![CDATA[News RSS Feed]]></title>
        <description><![CDATA[Best news site in the world!]]></description>
        <link>https://news.ctf.bsidestlv.com/rss.xml</link>
        <generator>RSS for Node</generator>
        <lastBuildDate>Fri, 23 Jul 2021 06:01:36 GMT</lastBuildDate>
        <atom:link href="https://news.ctf.bsidestlv.com/rss.xml" rel="self" type="application/rss+xml"/>
        <language><![CDATA[en]]></language>
        <ttl>60</ttl>
        <category><![CDATA[news]]></category>
        <item>
            <title><![CDATA[Main site]]></title>
            <description><![CDATA[Best news site in the world!]]></description>
            <link>https://news.ctf.bsidestlv.com/</link>
            <guid isPermaLink="true">https://news.ctf.bsidestlv.com/</guid>
            <category><![CDATA[News]]></category>
        </item>
        <item>
            <title><![CDATA[About Us]]></title>
            <description><![CDATA[Best news site in the world!]]></description>
            <link>https://news.ctf.bsidestlv.com/aboutus</link>
            <guid isPermaLink="true">https://news.ctf.bsidestlv.com/aboutus</guid>
            <category><![CDATA[News]]></category>
            <category><![CDATA[About us]]></category>
        </item>
        <item>
            <title><![CDATA[Art]]></title>
            <description><![CDATA[Best news site in the world!]]></description>
            <link>https://news.ctf.bsidestlv.com/art</link>
            <guid isPermaLink="true">https://news.ctf.bsidestlv.com/art</guid>
            <category><![CDATA[News]]></category>
            <category><![CDATA[Art]]></category>
        </item>
        <item>
            <title><![CDATA[Author]]></title>
            <description><![CDATA[Best news site in the world!]]></description>
            <link>https://news.ctf.bsidestlv.com/author</link>
            <guid isPermaLink="true">https://news.ctf.bsidestlv.com/author</guid>
            <category><![CDATA[News]]></category>
            <category><![CDATA[author]]></category>
        </item>
        <item>
            <title><![CDATA[Contact Us]]></title>
            <description><![CDATA[Best news site in the world!]]></description>
            <link>https://news.ctf.bsidestlv.com/contactus</link>
            <guid isPermaLink="true">https://news.ctf.bsidestlv.com/contactus</guid>
            <category><![CDATA[News]]></category>
            <category><![CDATA[Contact Us]]></category>
        </item>
        <item>
            <title><![CDATA[Fashion]]></title>
            <description><![CDATA[Best news site in the world!]]></description>
            <link>https://news.ctf.bsidestlv.com/fashion</link>
            <guid isPermaLink="true">https://news.ctf.bsidestlv.com/fashion</guid>
            <category><![CDATA[News]]></category>
            <category><![CDATA[Fashion]]></category>
        </item>
        <item>
            <title><![CDATA[News Post]]></title>
            <description><![CDATA[Best news site in the world!]]></description>
            <link>https://news.ctf.bsidestlv.com/news-post</link>
            <guid isPermaLink="true">https://news.ctf.bsidestlv.com/news-post</guid>
            <category><![CDATA[News]]></category>
            <category><![CDATA[Post]]></category>
        </item>
        <item>
            <title><![CDATA[Politics]]></title>
            <description><![CDATA[Best news site in the world!]]></description>
            <link>https://news.ctf.bsidestlv.com/politics</link>
            <guid isPermaLink="true">https://news.ctf.bsidestlv.com/politics</guid>
            <category><![CDATA[News]]></category>
            <category><![CDATA[Politics]]></category>
        </item>
        <item>
            <title><![CDATA[Real Estate]]></title>
            <description><![CDATA[Best news site in the world!]]></description>
            <link>https://news.ctf.bsidestlv.com/real-estate</link>
            <guid isPermaLink="true">https://news.ctf.bsidestlv.com/real-estate</guid>
            <category><![CDATA[News]]></category>
            <category><![CDATA[Real Estate]]></category>
        </item>
        <item>
            <title><![CDATA[Sports]]></title>
            <description><![CDATA[Best news site in the world!]]></description>
            <link>https://news.ctf.bsidestlv.com/sports</link>
            <guid isPermaLink="true">https://news.ctf.bsidestlv.com/sports</guid>
            <category><![CDATA[News]]></category>
            <category><![CDATA[Sports]]></category>
        </item>
        <item>
            <title><![CDATA[Flag]]></title>
            <description><![CDATA[Best news site in the world!]]></description>
            <link>https://news.ctf.bsidestlv.com/Im4Fl4gS3cr3t</link>
            <guid isPermaLink="true">https://news.ctf.bsidestlv.com/Im4Fl4gS3cr3t</guid>
            <category><![CDATA[News]]></category>
            <category><![CDATA[FLAG]]></category>
        </item>
        <item>
            <title><![CDATA[Travel]]></title>
            <description><![CDATA[Best news site in the world!]]></description>
            <link>https://news.ctf.bsidestlv.com/travel</link>
            <guid isPermaLink="true">https://news.ctf.bsidestlv.com/travel</guid>
            <category><![CDATA[News]]></category>
            <category><![CDATA[travel]]></category>
        </item>
        <item>
            <title><![CDATA[World]]></title>
            <description><![CDATA[Best news site in the world!]]></description>
            <link>https://news.ctf.bsidestlv.com/world</link>
            <guid isPermaLink="true">https://news.ctf.bsidestlv.com/world</guid>
            <category><![CDATA[News]]></category>
            <category><![CDATA[World]]></category>
        </item>
    </channel>
</rss>
```

Filtering just for the flag, we get:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/News]
└─$ curl -s https://news.ctf.bsidestlv.com/rss.xml | grep -i flag -A 3
            <title><![CDATA[Flag]]></title>
            <description><![CDATA[Best news site in the world!]]></description>
            <link>https://news.ctf.bsidestlv.com/Im4Fl4gS3cr3t</link>
            <guid isPermaLink="true">https://news.ctf.bsidestlv.com/Im4Fl4gS3cr3t</guid>
--
            <category><![CDATA[FLAG]]></category>
        </item>
        <item>
            <title><![CDATA[Travel]]></title>
```

We visit this URL and get the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/News]
└─$ curl https://news.ctf.bsidestlv.com/Im4Fl4gS3cr3t
BSidesTLV2021{D0ntSt0r3S3ns1t1v3F1l3s1NR55!}    
```