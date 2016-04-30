


function wsurl(s) {
    var l = window.location;
    return ((l.protocol === "https:") ? "wss://" : "ws://") + l.host + l.pathname + s;
}

jQuery.extend({
    putJSON: function( url, data, callback ){
        return $.ajax({
            type: 'put',
            url: url,
            processData: false,
            data: data,
            success: callback,
            contentType: 'application/json',
            dataType:    'json'
        });
    }
});

$(document).ready(function() {
      $.getJSON("/modes_CEA.json", function(config){
        var options = '';
        for (var i = 0; i < config.length; i++) {
            var wid=config[i].width;
            var hig=config[i].height;
            options += '<option ' + ' value="' + config[i].code + '">' + config[i].code + '_'  + config[i].width + '*' + config[i].height +  "-" + config[i].rate +  "-" + config[i].scan + '</option>';
        }
        $("#cea").html(options);
        //console.log(options);
      });

      $.getJSON("/modes_DMT.json", function(config){
        var options = '';
        for (var i = 0; i < config.length; i++) {
            options += '<option value="' + config[i].code + '">' + config[i].code + '_'  + config[i].width + '*' + config[i].height + "-" + config[i].rate +  "-" + config[i].scan + '</option>';
        }
        $("#dmt").html(options);
        //console.log(options);
      });



      // Websockets dont work on the arm-pi
      //var socket = new WebSocket(wsurl("socket/config/"));

      //socket.onopen = function(){
      //    console.log("Socket has been opened!");
      //}

      //socket.onmessage = function(msg){
      //    console.log("socket msg",msg);
      //}

      //socket.onmessage = function(msg){
      //    console.log("socket msg",msg);
      //}

      //socket.onclose = function(){
      //    console.log('Socket Status: '+socket.readyState+' (Closed)');
      //}


      var i=0;

var canvas = document.getElementById('canv'),
    ctx = canvas.getContext('2d'),   
    mulrect = [{"x":176,"y":64,"w":600,"h":400,"name":"video","color":"blue"},
               {"x":0,"y":0,"w":71,"h":57,"name":"logo","color":"grey"},
               {"x":82,"y":0,"w":706,"h":61,"name":"label","color":"cyan"},
               {"x":3,"y":66,"w":166,"h":329,"name":"list","color":"green"},
               {"x":0,"y":405,"w":167,"h":103,"name":"no_smoke","color":"red"},
               {"x":20,"y":522,"w":769,"h":73,"name":"scroll","color":"cyan"}],
    handlesSize = 16,
    currentHandle = false,
    drag = false;
    currentSel = 0;
    console.log('canv',canv);


   $.getJSON("/config.json", function(config){
     //mulrect=JSON.parse(config);
     mulrect=config;
     console.log("----------------------------");
     console.log(config);
     draw();
   });



function init() {
    canvas.addEventListener('mousedown', mouseDown, false);
    canvas.addEventListener('mouseup', mouseUp, false);
    canvas.addEventListener('mousemove', mouseMove, false);

    $("#ceaset").click(function(e){
       //$("#canv").
       $.ajax({
                  "dataType": 'json',
                  "type": "POST",
                  "url": "/api/mode",
                  "data": { "type": "CEA", "mode" : $( "#cea" ).val() },
                  "complete": function (response) {
                                  console.log('complete');
                                  //$('#output').html(response.responseText);
                              }
       });
    });

      $("#getconf").click(function(e){
        var str=JSON.stringify(mulrect,undefined,2);
        document.getElementById('conf').innerHTML = str
        // $('#conf').innerHtml(str);
      });


      $("#sb").click(function(e){
         test = { config : mulrect };
         //console.log('Socket Status: '+socket.readyState+' ??');
         // Only send on status open
         //socket.send(JSON.stringify(mulrect));
         console.log('click');



         function saveConfig() {
             //$('#output').html("Making AJAX call");
             $.ajax({
                        "dataType"    : 'json',
                        "type"        : "put",
                        "url"         : "/api/save",
                        contentType   : 'application/json',
                        "processData" : false,
                        "data":  JSON.stringify(mulrect) ,
                        "complete": function (response) {
                            console.log('complete save');
                            //$('#output').html(response.responseText);
                        }
                    });
         };
         saveConfig();

   });


   draw();
}

function point(x, y) {
    return {
	x: x,
	y: y
    };
}

function dist(p1, p2) {
    return Math.sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

function getHandle(mouse) {
    for (i=0;i<mulrect.length;i++) {
    currentSel=i;
    if (dist(mouse, point(mulrect[i].x, mulrect[i].y)) <= handlesSize) return 'topleft';
    if (dist(mouse, point(mulrect[i].x + mulrect[i].w, mulrect[i].y)) <= handlesSize) return 'topright';
    if (dist(mouse, point(mulrect[i].x, mulrect[i].y + mulrect[i].h)) <= handlesSize) return 'bottomleft';
    if (dist(mouse, point(mulrect[i].x + mulrect[i].w, mulrect[i].y + mulrect[i].h)) <= handlesSize) return 'bottomright';
    if (dist(mouse, point(mulrect[i].x + mulrect[i].w / 2, mulrect[i].y)) <= handlesSize) return 'top';
    if (dist(mouse, point(mulrect[i].x, mulrect[i].y + mulrect[i].h / 2)) <= handlesSize) return 'left';
    if (dist(mouse, point(mulrect[i].x + mulrect[i].w / 2, mulrect[i].y + mulrect[i].h)) <= handlesSize) return 'bottom';
    if (dist(mouse, point(mulrect[i].x + mulrect[i].w, mulrect[i].y + mulrect[i].h / 2)) <= handlesSize) return 'right';
    }

    i=0;
    return false;
}

function mouseDown(e) {
    if (currentHandle) drag = true;
    draw();
}

function mouseUp() {
    drag = false;
    currentHandle = false;
    draw();
}

function mouseMove(e) {
    var previousHandle = currentHandle;
    if (!drag) currentHandle = getHandle(point(e.pageX - this.offsetLeft, e.pageY - this.offsetTop));
    if (currentHandle && drag) {
	var mousePos = point(e.pageX - this.offsetLeft, e.pageY - this.offsetTop);
    i=currentSel;
	switch (currentHandle) {
	case 'topleft':
        mulrect[i].w += mulrect[i].x - mousePos.x;
        mulrect[i].h += mulrect[i].y - mousePos.y;
        mulrect[i].x = mousePos.x;
        mulrect[i].y = mousePos.y;
	    break;
	case 'topright':
        mulrect[i].w = mousePos.x - mulrect[i].x;
        mulrect[i].h += mulrect[i].y - mousePos.y;
        mulrect[i].y = mousePos.y;
	    break;
	case 'bottomleft':
        mulrect[i].w += mulrect[i].x - mousePos.x;
        mulrect[i].x = mousePos.x;
        mulrect[i].h = mousePos.y - mulrect[i].y;
	    break;
	case 'bottomright':
        mulrect[i].w = mousePos.x - mulrect[i].x;
        mulrect[i].h = mousePos.y - mulrect[i].y;
	    break;

	case 'top':
        mulrect[i].h += mulrect[i].y - mousePos.y;
        mulrect[i].y = mousePos.y;
	    break;

	case 'left':
        mulrect[i].w += mulrect[i].x - mousePos.x;
        mulrect[i].x = mousePos.x;
	    break;

	case 'bottom':
        mulrect[i].h = mousePos.y - mulrect[i].y;
	    break;

	case 'right':
        mulrect[i].w = mousePos.x - mulrect[i].x;
	    break;
	}
    }
    if (drag || currentHandle != previousHandle) draw();
}

function draw() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.strokeStyle = "blue";
    ctx.strokeRect(0, 0, canvas.width, canvas.height);
    ctx.strokeStyle = "black";

    ctx.fillStyle = 'black';
    ctx.textAlign='center';

    for (i=0;i<mulrect.length;i++)
    {
      ctx.fillStyle = mulrect[i].color;
      ctx.fillRect(mulrect[i].x, mulrect[i].y, mulrect[i].w, mulrect[i].h);
      ctx.fillStyle = 'black';
      ctx.strokeText(mulrect[i].name, mulrect[i].x+mulrect[i].w/2, mulrect[i].y+mulrect[i].h/2);

    }
    ctx.fillStyle = 'black';

    if (currentHandle) {
	var posHandle = point(0, 0);
	switch (currentHandle) {
	case 'topleft':
        posHandle.x = mulrect[i].x;
        posHandle.y = mulrect[i].y;
	    break;
	case 'topright':
        posHandle.x = mulrect[i].x + mulrect[i].w;
        posHandle.y = mulrect[i].y;
	    break;
	case 'bottomleft':
        posHandle.x = mulrect[i].x;
        posHandle.y = mulrect[i].y + mulrect[i].h;
	    break;
	case 'bottomright':
        posHandle.x = mulrect[i].x + mulrect[i].w;
        posHandle.y = mulrect[i].y + mulrect[i].h;
	    break;
	case 'top':
        posHandle.x = mulrect[i].x + mulrect[i].w / 2;
        posHandle.y = mulrect[i].y;
	    break;
	case 'left':
        posHandle.x = mulrect[i].x;
        posHandle.y = mulrect[i].y + mulrect[i].h / 2;
	    break;
	case 'bottom':
        posHandle.x = mulrect[i].x + mulrect[i].w / 2;
        posHandle.y = mulrect[i].y + mulrect[i].h;
	    break;
	case 'right':
        posHandle.x = mulrect[i].x + mulrect[i].w;
        posHandle.y = mulrect[i].y + mulrect[i].h / 2;
	    break;
	}
	ctx.globalCompositeOperation = 'xor';
	ctx.beginPath();
	ctx.arc(posHandle.x, posHandle.y, handlesSize, 0, 2 * Math.PI);
	ctx.fill();
	ctx.globalCompositeOperation = 'source-over';
    i=0;
    }
}

init();
draw();

});
