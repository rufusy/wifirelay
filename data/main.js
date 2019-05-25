var Socket;
window.onbeforeunload = function()
{
  Socket.onclose = function () {}; // disable onclose handler first
  Socket.close()
};

function createWs()
{
    Socket = new WebSocket('ws://'+window.location.hostname+':81/');
    Socket.onopen = function(evt) { console.log('websocket open');};
    Socket.onclose = function(evt) { console.log('websocket close');};
    Socket.onerror = function(evt) {console.log('WebSocket Error', evt);};
    Socket.onmessage = function(evt)
    {
        /****************************************************
        /* IO Page
        *******************************************************/
        if(document.getElementById('io_page').style.display != 'none')
        {
            console.log(evt.data);
            var ch1_wkp = document.getElementById('ch-1-startup-status'); // startup state
            var ch1_op = document.getElementById('ch-1-operating-status');  // operating state

            var ch2_wkp = document.getElementById('ch-2-startup-status');
            var ch2_op = document.getElementById('ch-2-operating-status');

            var ch3_wkp = document.getElementById('ch-3-startup-status');
            var ch3_op = document.getElementById('ch-3-operating-status');

            var ch4_wkp = document.getElementById('ch-4-startup-status');
            var ch4_op = document.getElementById('ch-4-operating-status');

            var ch5_wkp = document.getElementById('ch-5-startup-status');
            var ch5_op = document.getElementById('ch-5-operating-status');

            if(evt.data == '+ch1.high*') ch1_wkp.checked = true;
            else if(evt.data == '+ch1.low*') ch1_wkp.checked = false;
            else if(evt.data == '+ch1.on*') ch1_op.innerHTML = "ON";
            else if(evt.data == '+ch1.off*') ch1_op.innerHTML = "OFF";

            else if(evt.data == '+ch2.high*') ch2_wkp.checked = true;
            else if(evt.data == '+ch2.low*') ch2_wkp.checked = false;
            else if(evt.data == '+ch2.on*') ch2_op.innerHTML = "ON";
            else if(evt.data == '+ch2.off*') ch2_op.innerHTML = "OFF";

            else if(evt.data == '+ch3.high*')ch3_wkp.checked = true;
            else if(evt.data == '+ch3.low*')ch3_wkp.checked = false;
            else if(evt.data == '+ch3.on*')ch3_op.innerHTML = "ON";
            else if(evt.data == '+ch3.off*')ch3_op.innerHTML = "OFF";

            else if(evt.data == '+ch4.high*') ch4_wkp.checked = true;
            else if(evt.data == '+ch4.low*') ch4_wkp.checked = false;
            else if(evt.data == '+ch4.on*') ch4_op.innerHTML = "ON";
            else if(evt.data == '+ch4.off*') ch4_op.innerHTML = "OFF";

            else if(evt.data == '+ch5.high*') ch5_wkp.checked = true;
            else if(evt.data == '+ch5.low*') ch5_wkp.checked = false;
            else if(evt.data == '+ch5.on*') ch5_op.innerHTML = "ON";
            else if(evt.data == '+ch5.off*') ch5_op.innerHTML = "OFF";
            else console.log('unknown event');
        }

        /****************************************************
        /* Relay Page
        *******************************************************/
        if(document.getElementById('relay_page').style.display != 'none')
        {
            console.log(evt.data);
            var relay1_wkp = document.getElementById('relay-1-startup-status');
            var relay1_op = document.getElementById('relay-1-operating-status');

            var relay2_wkp = document.getElementById('relay-2-startup-status');
            var relay2_op = document.getElementById('relay-2-operating-status');

            var relay3_wkp = document.getElementById('relay-3-startup-status');
            var relay3_op = document.getElementById('relay-3-operating-status');

            var relay4_wkp = document.getElementById('relay-4-startup-status');
            var relay4_op = document.getElementById('relay-4-operating-status');

            var relay5_wkp = document.getElementById('relay-5-startup-status');
            var relay5_op = document.getElementById('relay-5-operating-status');


            if(evt.data == '+relay1.high*') relay1_wkp.checked = true;
            else if(evt.data == '+relay1.low*') relay1_wkp.checked = false;
            else if(evt.data == '+relay1.on*') relay1_op.innerHTML = "ON";
            else if(evt.data == '+relay1.off*') relay1_op.innerHTML = "OFF";

            else if(evt.data == '+relay2.high*') relay2_wkp.checked = true;
            else if(evt.data == '+relay2.low*') relay2_wkp.checked = false;
            else if(evt.data == '+relay2.on*') relay2_op.innerHTML = "ON";
            else if(evt.data == '+relay2.off*') relay2_op.innerHTML = "OFF";

            else if(evt.data == '+relay3.high*') relay3_wkp.checked = true;
            else if(evt.data == '+relay3.low*') relay3_wkp.checked = false;
            else if(evt.data == '+relay3.on*') relay3_op.innerHTML = "ON";
            else if(evt.data == '+relay3.off*') relay3_op.innerHTML = "OFF";

            else if(evt.data == '+relay4.high*') relay4_wkp.checked = true;
            else if(evt.data == '+relay4.low*') relay4_wkp.checked = false;
            else if(evt.data == '+relay4.on*') relay4_op.innerHTML = "ON";
            else if(evt.data == '+relay4.off*') relay4_op.innerHTML = "OFF";

            else if(evt.data == '+relay5.high*') relay5_wkp.checked = true;
            else if(evt.data == '+relay5.low*') relay5_wkp.checked = false;
            else if(evt.data == '+relay5.on*') relay5_op.innerHTML = "ON";
            else if(evt.data == '+relay5.off*') relay5_op.innerHTML = "OFF";

            else {
                //console.log('unknown event');
            }
        }
    };
}


function show_home_page()
{
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
    // all available pages
    var pages_list =    ["home_page",
                        "io_page",
                        "about_page",
                        "analog_page",
                        "auth_page",
                        "datetime_page",
                        "relay_page"];

    // we'll pick the page to display based on the arg passed  in the function
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


function gpio_startup_status(d)
{
    var checkbox_startup = document.getElementById(d.getAttribute('id'));
    if (checkbox_startup.checked)
        Socket.send('+'+d.getAttribute('name')+'.'+'high*');
    else
        Socket.send('+'+d.getAttribute('name')+'.'+'low*');
}


function relay_startup_status(d)
{
    var relay_startup =  document.getElementById(d.getAttribute('id'));
    if(relay_startup.checked)
        Socket.send('+'+d.getAttribute('name')+'.'+'high*');
    else
        Socket.send('+'+d.getAttribute('name')+'.'+'low*');
}


function relay_timer(d)
{
    var relay_btn = document.getElementById(d.getAttribute('id')).getAttribute("name");
    if(relay_btn == "relay1-save")
    {
        var relay1_on_time = document.getElementById("relay1-on").value;
        var relay1_off_time = document.getElementById("relay1-off").value;
        Socket.send("t.relay1."+relay1_on_time+"."+relay1_off_time);
    }
    else if(relay_btn == "relay2-save")
    {
        var relay2_on_time = document.getElementById("relay2-on").value;
        var relay2_off_time = document.getElementById("relay2-off").value;
        Socket.send("t.relay2."+relay2_on_time+"."+relay2_off_time);
    }
    else if(relay_btn == "relay3-save")
    {
        var relay3_on_time = document.getElementById("relay3-on").value;
        var relay3_off_time = document.getElementById("relay3-off").value;
        Socket.send("t.relay3."+relay3_on_time+"."+relay3_off_time);
    }
    else if(relay_btn == "relay4-save")
    {
        var relay4_on_time = document.getElementById("relay4-on").value;
        var relay4_off_time = document.getElementById("relay4-off").value;
        Socket.send("t.relay4."+relay4_on_time+"."+relay4_off_time);
    }
    else if(relay_btn == "relay5-save")
    {
        var relay5_on_time = document.getElementById("relay5-on").value;
        var relay5_off_time = document.getElementById("relay5-off").value;
        Socket.send("t.relay5."+relay5_on_time+"."+relay5_off_time);
    }
    else
    {
        // error
    }
}

function analog(d)
{
    var analog_btn = document.getElementById(d.getAttribute('id')).getAttribute("name");
    if(analog_btn == "an1-save")
    {
        var an1_up_val = document.getElementById("an1-up-val").value;
        var an1_low_val = document.getElementById("an1-low-val").value;
        var an1_out1 = document.getElementById("an1-output1").value;
        var an1_out2 = document.getElementById("an1-output2").value;
        var an1_out1_action = document.getElementById("an1-output1-action").value;
        var an1_out2_action = document.getElementById("an1-output2-action").value;
        Socket.send("+an1."+an1_up_val+"."+an1_low_val+"."+an1_out1+"."+an1_out2+"."+
            an1_out1_action+"."+an1_out2_action+"*");
    }
    else if(analog_btn == "an2-save")
    {
        var an2_up_val = document.getElementById("an2-up-val").value;
        var an2_low_val = document.getElementById("an2-low-val").value;
        var an2_out1 = document.getElementById("an2-output1").value;
        var an2_out2 = document.getElementById("an2-output2").value;
        var an2_out1_action = document.getElementById("an2-output1-action").value;
        var an2_out2_action = document.getElementById("an2-output2-action").value;
        Socket.send("+an2."+an2_up_val+"."+an2_low_val+"."+an2_out1+"."+an2_out2+"."+
            an2_out1_action+"."+an2_out2_action+"*");
    
    }
    else if(analog_btn == "an3-save")
    {
        var an3_up_val = document.getElementById("an3-up-val").value;
        var an3_low_val = document.getElementById("an3-low-val").value;
        var an3_out1 = document.getElementById("an3-output1").value;
        var an3_out2 = document.getElementById("an3-output2").value;
        var an3_out1_action = document.getElementById("an3-output1-action").value;
        var an3_out2_action = document.getElementById("an3-output2-action").value;
        Socket.send("+an3."+an3_up_val+"."+an3_low_val+"."+an3_out1+"."+an3_out2+"."+
            an3_out1_action+"."+an3_out2_action+"*");
    }
    else if(analog_btn == "an4-save")
    {
        var an4_up_val = document.getElementById("an4-up-val").value;
        var an4_low_val = document.getElementById("an4-low-val").value;
        var an4_out1 = document.getElementById("an4-output1").value;
        var an4_out2 = document.getElementById("an4-output2").value;
        var an4_out1_action = document.getElementById("an4-output1-action").value;
        var an4_out2_action = document.getElementById("an4-output2-action").value;
        Socket.send("+an4."+an4_up_val+"."+an4_low_val+"."+an4_out1+"."+an4_out2+"."+
            an4_out1_action+"."+an4_out2_action+"*");
    }
    else
    {
        //
    }
}
