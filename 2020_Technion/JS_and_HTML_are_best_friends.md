# JS and HTML are best friends
Category: Web

## Description
> Part 3 of Imperva's challenge
> 
> Put your JavaScript to the test and manipulate the DOM!
> 

## Solution

The [previous challenge](Hard_login.md)'s flag has left us with a hint: `the_company_likes_cookies`.

We create an account by entering a desired username and password. Then, we login to the bug bounty management page.

![](images/bug_bounty.png)

In this page we have several buttons. We'll concentrate on the "Add bug" functionality for this writeup. The other buttons are related to other challenges and we'll see them later.

After clicking the "Add bug" button, we get a popup which allows us to submit a bug. We need to select the category from a dropdown menu, and then enter the relevant company and the bug details.

After some trial and error, we discover that the "Company" field is vulnerable to XSS. Relevant code on client side is:

```javascript
function populateTable(data) {
    // $('#bugsTable').bootstrapTable({
    //     data: datadata
    // });
    for (var i = 0; i < data.length; i++) {
        var bugDetails = data[i].details
        var pattern = /((http|https|ftp):\/\/)/;
        if(pattern.test(bugDetails)) {
            let docURL = bugDetails.substring(0, bugDetails.lastIndexOf('|'))
            let docName = bugDetails.substring(bugDetails.lastIndexOf('/')+1, bugDetails.lastIndexOf('|'))
            let creator = bugDetails.substring(bugDetails.lastIndexOf('|')+1, bugDetails.length)
            bugDetails = `<a href=${docURL}>${docName}</a> by ${creator}`
        }
        var row = $('<tr><td class="align-middle">' + data[i].category + '</td><td class="align-middle">' + data[i].company + '</td><td class="align-middle">' + bugDetails + '</td></tr>');
        $('#bugsTable').append(row);
    }
}

```

If we provide a `<script>` in the company field, JQuery's element creation syntax (`$('<element>')`) will include it in the element creation, and `$('#bugsTable').append(row)` will attach it to the DOM and execute it. There is no escaping done on the server side either.

Remember that the hint said "The company likes cookies". This is not so clear or intuitive, but based on the challenge description and hint it turns out that in order to get the flag, we need to use the XSS vulnerability to write our cookie to the "company" table entry.

Here's our table after submitting a few bugs:

```html
<table class="table table-striped table-bordered" id="bugsTable">
    <thead class="thead-dark">
        <tr>
            <th scope="col" data-field="category" class="text-center">Category</th>
            <th scope="col" data-field="company" class="text-center">Company</th>
            <th scope="col" data-field="details" class="text-center">Details</th>
        </tr>
    </thead>
    <tbody id="bugsTableData"><tr><td class="align-middle">XSS</td><td class="align-middle">Imperva</td><td class="align-middle"></td></tr><tr><td class="align-middle">SSRF</td><td class="align-middle">Imperva</td><td class="align-middle"></td></tr></tbody>
</table>
```

So, in order to modify the "Company" entry of one of the rows, we submit the following Javascript snippet as a "company":
```javascript
<script>document.getElementById("bugsTable").rows[1].cells[1].innerHTML = document.cookie; </script>
```

After clicking the "Show bugs reports" button again to refresh the table, we get see that the Javascript has been executed and get the flag: `cstechnion{i_can_view_other_bugs}`. 

The flag is a hint for the [next challenge](Other_bugs.md).