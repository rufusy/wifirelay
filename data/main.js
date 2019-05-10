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
            var ch1_dir = document.getElementById('ch-1-direction');
            var ch1_wkp = document.getElementById('ch-1-startup-status');
            var ch1_op = document.getElementById('ch-1-operating-status');

            var ch2_dir = document.getElementById('ch-2-direction');
            var ch2_wkp = document.getElementById('ch-2-startup-status');
            var ch2_op = document.getElementById('ch-2-operating-status');

            var ch3_dir = document.getElementById('ch-3-direction');
            var ch3_wkp = document.getElementById('ch-3-startup-status');
            var ch3_op = document.getElementById('ch-3-operating-status');

            var ch4_dir = document.getElementById('ch-4-direction');
            var ch4_wkp = document.getElementById('ch-4-startup-status');
            var ch4_op = document.getElementById('ch-4-operating-status');

            var ch5_dir = document.getElementById('ch-5-direction');
            var ch5_wkp = document.getElementById('ch-5-startup-status');
            var ch5_op = document.getElementById('ch-5-operating-status');

            if(evt.data == '+ch1.output*') ch1_dir.checked = true;
            else if(evt.data == '+ch1.input*') ch1_dir.checked = false;
            else if(evt.data == '+ch1.high*') ch1_wkp.checked = true;
            else if(evt.data == '+ch1.low*') ch1_wkp.checked = false;
            else if(evt.data == '+ch1.on*') ch1_op.checked = true;
            else if(evt.data == '+ch1.off*') ch1_op.checked = false;

            else if(evt.data == '+ch2.output*') ch2_dir.checked = true;
            else if(evt.data == '+ch2.input*') ch2_dir.checked = false;
            else if(evt.data == '+ch2.high*') ch2_wkp.checked = true;
            else if(evt.data == '+ch2.low*') ch2_wkp.checked = false;
            else if(evt.data == '+ch2.on*') ch2_op.checked = true;
            else if(evt.data == '+ch2.off*') ch2_op.checked = false;

            else if(evt.data == '+ch3.output*') ch3_dir.checked = true;
            else if(evt.data == '+ch3.input*') ch3_dir.checked = false;
            else if(evt.data == '+ch3.high*')ch3_wkp.checked = true;
            else if(evt.data == '+ch3.low*')ch3_wkp.checked = false;
            else if(evt.data == '+ch3.on*')ch3_op.checked = true;
            else if(evt.data == '+ch3.off*')ch3_op.checked = false;

            else if(evt.data == '+ch4.output*') ch4_dir.checked = true;
            else if(evt.data == '+ch4.input*') ch4_dir.checked = false;
            else if(evt.data == '+ch4.high*') ch4_wkp.checked = true;
            else if(evt.data == '+ch4.low*') ch4_wkp.checked = false;
            else if(evt.data == '+ch4.on*') ch4_op.checked = true;
            else if(evt.data == '+ch4.off*') ch4_op.checked = false;

            else if(evt.data == '+ch5.output*') ch5_dir.checked = true;
            else if(evt.data == '+ch5.input*') ch5_dir.checked = false;
            else if(evt.data == '+ch5.high*') ch5_wkp.checked = true;
            else if(evt.data == '+ch5.low*') ch5_wkp.checked = false;
            else if(evt.data == '+ch5.on*') ch5_op.checked = true;
            else if(evt.data == '+ch5.off*') ch5_op.checked = false;

            else console.log('unknown event');
        }

        /****************************************************
        /* Relay Page
        *******************************************************/
        if(document.getElementById('relay_page').style.display != 'none')
        {
            console.log(evt.data);
            var relay1_wkp = document.getElementById('relay-1-startup-status');
            var relay2_wkp = document.getElementById('relay-2-startup-status');
            var relay3_wkp = document.getElementById('relay-3-startup-status');
            var relay4_wkp = document.getElementById('relay-4-startup-status');
            var relay5_wkp = document.getElementById('relay-5-startup-status');

            if(evt.data == '+relay1.high*') relay1_wkp.checked = true;
            else if(evt.data == '+relay1.low*') relay1_wkp.checked = false;

            else if(evt.data == '+relay2.high*') relay2_wkp.checked = true;
            else if(evt.data == '+relay2.low*') relay2_wkp.checked = false;

            else if(evt.data == '+relay3.high*') relay3_wkp.checked = true;
            else if(evt.data == '+relay3.low*') relay3_wkp.checked = false;

            else if(evt.data == '+relay4.high*') relay4_wkp.checked = true;
            else if(evt.data == '+relay4.low*') relay4_wkp.checked = false;

            else if(evt.data == '+relay5.high*') relay5_wkp.checked = true;
            else if(evt.data == '+relay5.low*') relay5_wkp.checked = false;

            else console.log('unknown event');
        }
    };
}


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




function gpio_set_dir(d)
{
    var io_dir_config = {};
    var io_dir_cmd;

    var checkbox_dir = document.getElementById(d.getAttribute('id'));
    if (checkbox_dir.checked)
    {
        //io_dir_config['io'] = d.getAttribute('name');
        //io_dir_config['dir'] = 'output';
        Socket.send('+'+d.getAttribute('name')+'.'+'output*')
    }
    else
    {
        //io_dir_config['io'] = d.getAttribute('name');
        //io_dir_config['dir'] = 'input';
        Socket.send('+'+d.getAttribute('name')+'.'+'input*')
    }
    //io_dir_cmd = JSON.stringify(io_dir_config);
    //alert(io_dir_cmd);
    //console.log(io_dir_cmd);
    //Socket.send(io_dir_cmd);
}



function gpio_startup_status(d)
{
    var io_startup_config = {};
    var io_startup_cmd;

    var checkbox_startup = document.getElementById(d.getAttribute('id'));
    if (checkbox_startup.checked)
    {
        //io_startup_config['io'] = d.getAttribute('name');
        //io_startup_config['startup'] = 'high';
        Socket.send('+'+d.getAttribute('name')+'.'+'high*')
    }
    else
    {
        //io_startup_config['io'] = d.getAttribute('name');
        //io_startup_config['startup'] = 'low';
        Socket.send('+'+d.getAttribute('name')+'.'+'low*')
    }
    //io_startup_cmd = JSON.stringify(io_startup_config);
    //alert(io_startup_cmd);
    //console.log(io_startup_cmd);
    //Socket.send(io_startup_cmd);
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
    var relay = document.getElementById(d.getAttribute('id'));
    Socket.send('+'+relay.getAttribute("id")+'.'+relay.value+'*');
}


function io_page_updater()
{

}

function relay_page_updater()
{
}
