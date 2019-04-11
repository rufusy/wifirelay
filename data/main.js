
function show_home_page()
{
    //document.querySelector("#index_page").style.display = " ";
    document.getElementById("home_page").style.display = "";
    document.getElementById("io_page").style.display = "none";
    document.getElementById("about_page").style.display = "none";
    document.getElementById("analog_page").style.display = "none";
    document.getElementById("auth_page").style.display = "none";
    document.getElementById("datetime_page").style.display = "none";
    document.getElementById("relay_page").style.display = "none";
}


function show_page(d)
{
    /*
        list all available pages
    */
    var pages_list =    ["home_page",
                        "io_page",
                        "about_page",
                        "analog_page",
                        "auth_page",
                        "datetime_page",
                        "relay_page"];
    /*
        we'll pick the page to display based on the arg passed
        in the function
    */
    var page_id = d.getAttribute('id');
    var page_name = d.getAttribute('name');
    var page_index = pages_list.indexOf(page_name);
    //alert(page_id+','+page_name+','+page_index);
    var i;
    for (i=0; i<pages_list.length; i++)
    {
        if(i == page_index) // eg 4 == 4 == name:auth_page -> id:auth_page
            document.getElementById(pages_list[i]).style.display = "";
        else
            document.getElementById(pages_list[i]).style.display = "none";
    }
}


function includeHTML() {
    var z, i, elmnt, file, xhttp;
    /* Loop through a collection of all HTML elements: */
    z = document.getElementsByTagName("*");
    for (i = 0; i < z.length; i++) {
        elmnt = z[i];
        /*search for elements with a certain atrribute:*/
        file = elmnt.getAttribute("w3-include-html");
        if (file) {
            /* Make an HTTP request using the attribute value as the file name: */
            xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4) {
                    if (this.status == 200) {elmnt.innerHTML = this.responseText;}
                    if (this.status == 404) {elmnt.innerHTML = "Page not found.";}
                    /* Remove the attribute, and call this function once more: */
                    elmnt.removeAttribute("w3-include-html");
                    includeHTML();
                }
            }
            xhttp.open("GET", file, true);
            xhttp.send();
            /* Exit the function: */
            return;
        }
    }
}
