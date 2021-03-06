#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Please create credentials.h to store credentials
/*
#ifndef STATION_SSID

#define STATION_SSID "ssid"
#define STATION_PSK "passwd"

#endif
*/

#include "credentials.h"

const char* ssid     = STATION_SSID;
const char* password = STATION_PSK;

ESP8266WebServer server(80);

const int led = LED_BUILTIN;

/**
 * Color palette: https://www.color-hex.com/color-palette/25362
 */
#define MULTI_LINE_STRING(...) #__VA_ARGS__

static const char postForms[] PROGMEM = MULTI_LINE_STRING(
<!DOCTYPE html>
<meta charset="UTF-8">
<html lang="en">
 <head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <link rel="stylesheet" type="text/css" href="milligram.min.css" />
  <style>
    .center,.button,input[type="submit"]{
      display: flex;
      justify-content: center;
      align-items: center;
      margin: 0 auto;
    }
    .slidecontainer {
      width: 100%;
    }
    
    .slider {
      -webkit-appearance: none;
      width: 100%;
      height: 25px;
      background: #d3d3d3;
      outline: none;
      opacity: 0.7;
      -webkit-transition: .2s;
      transition: opacity .2s;
    }
    
    .slider:hover {
      opacity: 1;
    }
    
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 25px;
      height: 25px;
      background: #4CAF50;
      cursor: pointer;
    }
    
    .slider::-moz-range-thumb {
      width: 25px;
      height: 25px;
      background: #4CAF50;
      cursor: pointer;
    }
  </style>
  <script src="kelly.js"></script>
  <script type="text/javascript">
   function changeColor() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "submit", true); 
    xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    
    var selector = "";
    if(document.getElementById("colorselect").value == "primary") {
      selector = "1 "
    }
    else if(document.getElementById("colorselect").value == "secondary") {
      selector = "2 "
    }
    
    var data = "color " + selector + "0x" + document.getElementById("color").value.replace("#", "");
    xhttp.send("command=" + encodeURIComponent(data).replace(/%20/g,'+'));
   }
   
   function changePattern() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "submit", true); 
    xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    var data = "pattern " + document.getElementById("patternselect").value;
    xhttp.send("command=" + encodeURIComponent(data).replace(/%20/g,'+'));
   }

   function changeSpeed() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "submit", true); 
    xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    var data = "speed " + document.getElementById("speed").value;
    xhttp.send("command=" + encodeURIComponent(data).replace(/%20/g,'+'));
   }

   function changeBrightness() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "submit", true); 
    xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    var data = "brightness " + document.getElementById("brightness").value;
    xhttp.send("command=" + encodeURIComponent(data).replace(/%20/g,'+'));
   }
  </script>
 </head>
 <body>
  <h1>Hello World</h1>
  <form method="post" action="submit">
   <label for="command">Raw Command:</label>
   <input type="text" name="command" value=""/></br>
   <label for="speed">Animation speed:</label>
   <input type="range" min="15" max="150" value="50" class="slider" id="speed" onchange="changeSpeed()">
   <label for="brightness">Brightness:</label>
   <input type="range" min="1" max="255" value="100" class="slider" id="brightness" onchange="changeBrightness()">
   <label for="patternselect">Pattern:</label>
   <select name="patternselect" id="patternselect" onchange="changePattern()">
    <option value="COLOR_SOLID">Solid color</option>
    <option value="STROBE_SOLID">Strobe</option>
    <option value="SUNRISE">Sunrise</option>
    <option value="RAINBOW">Rainbow</option>
    <option value="COLOR_WIPE_SOLID">Color wipe</option>
    <option value="COLOR_WIPE_CHRISTMAS">Christmas color wipe</option>
    <option value="COLOR_WIPE_RANDOM">Random color wipe</option>
    <option value="SCANNER_SOLID">Scanner</option>
    <option value="SCANNER_RANDOM">Random scanner</option>
    <option value="FADE_SOLID">Fade</option>
    <option value="FADE_RANDOM">Random fade</option>
    <option value="FIRE_SOLID">Fire</option>
    <option value="FIRE_SCANNER">Random fire</option>
    <option value="METEOR_SOLID">Meteor</option>
    <option value="METEOR_SCANNER_SOLID">Meteor scanner</option>
   </select></br>
   <label for="colorselect">Color selection:</label>
   <select name="colorselect" id="colorselect">
    <option value="all">Both</option>
    <option value="primary">Primary</option>
    <option value="secondary">Secondary</option>
   </select></br>
   <label for="color">Color:</label>
   <input id="color" name="color" value="#000000" onchange="changeColor()"/></br>
   <div class="center" style="width:300px;">
    <canvas id="picker"></canvas></br>
   </div>
   <input type="submit" class="center" value="Test Submit"/>
  </form>
  <script type="text/javascript">
    let picker = new KellyColorPicker({
      place : 'picker', input : 'color', size : 300
    });

    picker.addUserEvent("mouseuph", changeColor);
    picker.addUserEvent("mouseupsv", changeColor);
  </script>
 </body>
</html>
);

static const char milligram[] PROGMEM = MULTI_LINE_STRING(
  *,*:after,*:before{box-sizing:inherit}html{box-sizing:border-box;font-size:62.5%}body{background-color:#23272a;color:#ffffff;font-family:'Roboto', 'Helvetica Neue', 'Helvetica', 'Arial', sans-serif;font-size:1.6em;font-weight:300;letter-spacing:.01em;line-height:1.6}input,select{color:#ffffff;}blockquote{border-left:0.3rem solid #d1d1d1;margin-left:0;margin-right:0;padding:1rem 1.5rem}blockquote *:last-child{margin-bottom:0}.button,button,input[type='button'],input[type='reset'],input[type='submit']{background-color:#7289da;border:0.1rem solid #7289da;border-radius:.4rem;color:#fff;cursor:pointer;display:inline-block;font-size:1.1rem;font-weight:700;height:3.8rem;letter-spacing:.1rem;line-height:3.8rem;padding:0 3.0rem;text-align:center;text-decoration:none;text-transform:uppercase;white-space:nowrap}.button:focus,.button:hover,button:focus,button:hover,input[type='button']:focus,input[type='button']:hover,input[type='reset']:focus,input[type='reset']:hover,input[type='submit']:focus,input[type='submit']:hover{background-color:#99aab5;border-color:#2c2f33;color:#fff;outline:0}.button[disabled],button[disabled],input[type='button'][disabled],input[type='reset'][disabled],input[type='submit'][disabled]{cursor:default;opacity:.5}.button[disabled]:focus,.button[disabled]:hover,button[disabled]:focus,button[disabled]:hover,input[type='button'][disabled]:focus,input[type='button'][disabled]:hover,input[type='reset'][disabled]:focus,input[type='reset'][disabled]:hover,input[type='submit'][disabled]:focus,input[type='submit'][disabled]:hover{background-color:#7289da;border-color:#7289da}.button.button-outline,button.button-outline,input[type='button'].button-outline,input[type='reset'].button-outline,input[type='submit'].button-outline{background-color:transparent;color:#7289da}.button.button-outline:focus,.button.button-outline:hover,button.button-outline:focus,button.button-outline:hover,input[type='button'].button-outline:focus,input[type='button'].button-outline:hover,input[type='reset'].button-outline:focus,input[type='reset'].button-outline:hover,input[type='submit'].button-outline:focus,input[type='submit'].button-outline:hover{background-color:transparent;border-color#99aab5;color#99aab5}.button.button-outline[disabled]:focus,.button.button-outline[disabled]:hover,button.button-outline[disabled]:focus,button.button-outline[disabled]:hover,input[type='button'].button-outline[disabled]:focus,input[type='button'].button-outline[disabled]:hover,input[type='reset'].button-outline[disabled]:focus,input[type='reset'].button-outline[disabled]:hover,input[type='submit'].button-outline[disabled]:focus,input[type='submit'].button-outline[disabled]:hover{border-color:inherit;color:#7289da}.button.button-clear,button.button-clear,input[type='button'].button-clear,input[type='reset'].button-clear,input[type='submit'].button-clear{background-color:transparent;border-color:transparent;color:#7289da}.button.button-clear:focus,.button.button-clear:hover,button.button-clear:focus,button.button-clear:hover,input[type='button'].button-clear:focus,input[type='button'].button-clear:hover,input[type='reset'].button-clear:focus,input[type='reset'].button-clear:hover,input[type='submit'].button-clear:focus,input[type='submit'].button-clear:hover{background-color:transparent;border-color:transparent;color:#99aab5}.button.button-clear[disabled]:focus,.button.button-clear[disabled]:hover,button.button-clear[disabled]:focus,button.button-clear[disabled]:hover,input[type='button'].button-clear[disabled]:focus,input[type='button'].button-clear[disabled]:hover,input[type='reset'].button-clear[disabled]:focus,input[type='reset'].button-clear[disabled]:hover,input[type='submit'].button-clear[disabled]:focus,input[type='submit'].button-clear[disabled]:hover{color:#7289da}code{background:#f4f5f6;border-radius:.4rem;font-size:86%;margin:0 .2rem;padding:.2rem .5rem;white-space:nowrap}pre{background:#f4f5f6;border-left:0.3rem solid #7289da;overflow-y:hidden}pre>code{border-radius:0;display:block;padding:1rem 1.5rem;white-space:pre}hr{border:0;border-top:0.1rem solid #f4f5f6;margin:3.0rem 0}input[type='color'],input[type='date'],input[type='datetime'],input[type='datetime-local'],input[type='email'],input[type='month'],input[type='number'],input[type='password'],input[type='search'],input[type='tel'],input[type='text'],input[type='url'],input[type='week'],input:not([type]),textarea,select{-webkit-appearance:none;background-color:transparent;border:0.1rem solid #d1d1d1;border-radius:.4rem;box-shadow:none;box-sizing:inherit;height:3.8rem;padding:.6rem 1.0rem .7rem;width:100%}input[type='color']:focus,input[type='date']:focus,input[type='datetime']:focus,input[type='datetime-local']:focus,input[type='email']:focus,input[type='month']:focus,input[type='number']:focus,input[type='password']:focus,input[type='search']:focus,input[type='tel']:focus,input[type='text']:focus,input[type='url']:focus,input[type='week']:focus,input:not([type]):focus,textarea:focus,select:focus{border-color:#7289da;outline:0}select{background:url('data:image/svg+xml;utf8,<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 30 8" width="30"><path fill="%23d1d1d1" d="M0,0l6,8l6-8"/></svg>') center right no-repeat;padding-right:3.0rem}select:focus{background-image:url('data:image/svg+xml;utf8,<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 30 8" width="30"><path fill="%2#7289da" d="M0,0l6,8l6-8"/></svg>')}select[multiple]{background:none;height:auto}textarea{min-height:6.5rem}label,legend{display:block;font-size:1.6rem;font-weight:700;margin-bottom:.5rem}fieldset{border-width:0;padding:0}input[type='checkbox'],input[type='radio']{display:inline}.label-inline{display:inline-block;font-weight:normal;margin-left:.5rem}.container{margin:0 auto;max-width:112.0rem;padding:0 2.0rem;position:relative;width:100%}.row{display:flex;flex-direction:column;padding:0;width:100%}.row.row-no-padding{padding:0}.row.row-no-padding>.column{padding:0}.row.row-wrap{flex-wrap:wrap}.row.row-top{align-items:flex-start}.row.row-bottom{align-items:flex-end}.row.row-center{align-items:center}.row.row-stretch{align-items:stretch}.row.row-baseline{align-items:baseline}.row .column{display:block;flex:1 1 auto;margin-left:0;max-width:100%;width:100%}.row .column.column-offset-10{margin-left:10%}.row .column.column-offset-20{margin-left:20%}.row .column.column-offset-25{margin-left:25%}.row .column.column-offset-33,.row .column.column-offset-34{margin-left:33.3333%}.row .column.column-offset-40{margin-left:40%}.row .column.column-offset-50{margin-left:50%}.row .column.column-offset-60{margin-left:60%}.row .column.column-offset-66,.row .column.column-offset-67{margin-left:66.6666%}.row .column.column-offset-75{margin-left:75%}.row .column.column-offset-80{margin-left:80%}.row .column.column-offset-90{margin-left:90%}.row .column.column-10{flex:0 0 10%;max-width:10%}.row .column.column-20{flex:0 0 20%;max-width:20%}.row .column.column-25{flex:0 0 25%;max-width:25%}.row .column.column-33,.row .column.column-34{flex:0 0 33.3333%;max-width:33.3333%}.row .column.column-40{flex:0 0 40%;max-width:40%}.row .column.column-50{flex:0 0 50%;max-width:50%}.row .column.column-60{flex:0 0 60%;max-width:60%}.row .column.column-66,.row .column.column-67{flex:0 0 66.6666%;max-width:66.6666%}.row .column.column-75{flex:0 0 75%;max-width:75%}.row .column.column-80{flex:0 0 80%;max-width:80%}.row .column.column-90{flex:0 0 90%;max-width:90%}.row .column .column-top{align-self:flex-start}.row .column .column-bottom{align-self:flex-end}.row .column .column-center{align-self:center}@media (min-width: 40rem){.row{flex-direction:row;margin-left:-1.0rem;width:calc(100% + 2.0rem)}.row .column{margin-bottom:inherit;padding:0 1.0rem}}a{color:#7289da;text-decoration:none}a:focus,a:hover{color:#99aab5}dl,ol,ul{list-style:none;margin-top:0;padding-left:0}dl dl,dl ol,dl ul,ol dl,ol ol,ol ul,ul dl,ul ol,ul ul{font-size:90%;margin:1.5rem 0 1.5rem 3.0rem}ol{list-style:decimal inside}ul{list-style:circle inside}.button,button,dd,dt,li{margin-bottom:1.0rem}fieldset,input,select,textarea{margin-bottom:1.5rem}blockquote,dl,figure,form,ol,p,pre,table,ul{margin-bottom:2.5rem}table{border-spacing:0;display:block;overflow-x:auto;text-align:left;width:100%}td,th{border-bottom:0.1rem solid #e1e1e1;padding:1.2rem 1.5rem}td:first-child,th:first-child{padding-left:0}td:last-child,th:last-child{padding-right:0}@media (min-width: 40rem){table{display:table;overflow-x:initial}}b,strong{font-weight:bold}p{margin-top:0}h1,h2,h3,h4,h5,h6{font-weight:300;letter-spacing:-.1rem;margin-bottom:2.0rem;margin-top:0}h1{font-size:4.6rem;line-height:1.2}h2{font-size:3.6rem;line-height:1.25}h3{font-size:2.8rem;line-height:1.3}h4{font-size:2.2rem;letter-spacing:-.08rem;line-height:1.35}h5{font-size:1.8rem;letter-spacing:-.05rem;line-height:1.5}h6{font-size:1.6rem;letter-spacing:0;line-height:1.4}img{max-width:100%}.clearfix:after{clear:both;content:' ';display:table}.float-left{float:left}.float-right{float:right}
);

static const char kellypicker[] PROGMEM = MULTI_LINE_STRING(
  /*
    2015-2020 Rubchuk Vladimir
    @license   GPLv3
    @version   1.21
   */
   function KellyColorPicker(t){function i(t,i,e){i=!!i;var s={};s.width,s.widthPercentage=22,s.imageData=null,s.align=t,s.selected=i,s.color="#ffffff",s.position,s.paddingY=-4,s.paddingX=4,s.lineWidth=1,s.selectSize=4,"right"==t&&(s.paddingX*=-1),s.selected&&(s.color=J),e&&(s.color=e),s.updateSize=function(){this.width=parseInt(V-V/100*(100-this.widthPercentage)),"left"==this.align?this.position={x:0,y:V-this.width}:"right"==this.align&&(this.position={x:V-this.width,y:V-this.width})},s.calcS=function(t){return Math.abs((t[1].x-t[0].x)*(t[2].y-t[0].y)-(t[2].x-t[0].x)*(t[1].y-t[0].y))/2},s.isDotIn=function(t){var i=[];"left"==this.align?(i[0]={x:this.position.x,y:this.position.y},i[1]={x:this.position.x,y:this.position.y+this.width},i[2]={x:this.position.x+this.width,y:this.position.y+this.width}):(i[0]={x:this.position.x+this.width,y:this.position.y},i[1]={x:i[0].x,y:i[0].y+this.width},i[2]={x:i[0].x-this.width,y:this.position.y+this.width});for(var e=0;e<=i.length-1;++e)i[e].x+=this.paddingX,i[e].y+=this.paddingY;e=this.calcS(i),t=[{x:i[0].x,y:i[0].y},{x:i[1].x,y:i[1].y},{x:t.x,y:t.y}];var s=this.calcS(t);return t[1]={x:i[2].x,y:i[2].y},s+=this.calcS(t),t[0]={x:i[1].x,y:i[1].y},s+=this.calcS(t),Math.ceil(s)==Math.ceil(e)},s.draw=function(){A.width=this.width,A.height=this.width,q.clearRect(0,0,this.width,this.width),q.beginPath(),"left"==this.align&&(q.moveTo(this.lineWidth/2,this.width-this.lineWidth),q.lineTo(this.width,this.width-this.lineWidth),q.lineTo(this.lineWidth,this.lineWidth),q.lineTo(this.lineWidth,this.width-this.lineWidth)),"right"==this.align&&(q.moveTo(this.lineWidth/2,this.width-this.lineWidth),q.lineTo(this.width-this.lineWidth,this.width-this.lineWidth),q.lineTo(this.width-this.lineWidth,this.lineWidth),q.lineTo(this.lineWidth,this.width-this.lineWidth)),this.selected&&(q.fillStyle="rgba(255,255,255, 1)",q.fill(),q.strokeStyle="rgba(0, 0, 0, 1)",q.stroke(),q.closePath(),q.beginPath(),q.lineWidth=this.lineWidth,"left"==this.align&&(q.moveTo(this.selectSize,this.width-this.selectSize),q.lineTo(this.width-2*this.selectSize,this.width-this.selectSize),q.lineTo(this.selectSize,2*this.selectSize),q.lineTo(this.selectSize,this.width-this.selectSize)),"right"==this.align&&(q.moveTo(2*this.selectSize,this.width-this.selectSize),q.lineTo(this.width-this.selectSize,this.width-this.selectSize),q.lineTo(this.width-this.selectSize,2*this.selectSize),q.lineTo(2*this.selectSize,this.width-this.selectSize)));var t=u(this.color);q.fillStyle="rgba("+t.r+","+t.g+","+t.b+", 1)",q.fill(),q.strokeStyle="rgba(0, 0, 0, 1)",q.stroke(),this.imageData=q.getImageData(0,0,this.width,this.width),W.drawImage(A,this.position.x+this.paddingX,this.position.y+this.paddingY)},tt[tt.length]=s}function e(t){var i=t.getBoundingClientRect(),e=0,s=0;return R&&(s=at.width+2*at.padding),t===I?i.width<=i.height?e=i.height:i.height<i.width&&(e=i.width):$?"height"==$?e=i.height:"width"==$&&(e=i.width):i.width>i.height?e=i.height:i.height>=i.width&&(e=i.width),e=parseInt(e),R&&(e-=s),!(0>=e)&&e}function s(t,i){var e=1,s=!1;if("string"==typeof t){if(-1==(t=t.trim(t)).indexOf("(")){if("#"==t.charAt(0)&&(t=t.slice(1)),3<=(t=t.substr(0,8)).length){if(6<t.length&&8>t.length&&(t=t.substr(0,6)),3<t.length&&6>t.length&&(t=t.substr(0,3)),s=t,3<=t.length&&4>=t.length){s="";for(var o=0;o<t.length;o++)s+=t[o]+t[o]}8==s.length&&(e=(255&parseInt(s,16))/255)}}else if(vals=t.split(","),3<=vals.length){switch(t.substring(0,3)){case"rgb":vals[0]=vals[0].replace("rgba(",""),vals[0]=vals[0].replace("rgb(",""),255>=(o={r:parseInt(vals[0]),g:parseInt(vals[1]),b:parseInt(vals[2])}).r&&255>=o.g&&255>=o.b&&(s=c(o));break;case"hsl":vals[0]=vals[0].replace("hsl(",""),vals[0]=vals[0].replace("hsla(","");o=parseFloat(vals[0])/360;var a=parseFloat(vals[1])/100,h=parseFloat(vals[2])/100;0<=o&&1>=a&&1>=h&&(s=c(r(o,a,h)))}4==vals.length&&((!(e=parseFloat(vals[3]))||0>e)&&(e=0),1<e&&(e=1))}}else"object"==typeof t&&(void 0!==t.r&&void 0!==t.g&&void 0!==t.b?s=c(t):void 0!==t.h&&void 0!==t.s&&void 0!==t.l&&(t.h=parseFloat(t.h)/360,t.s=parseFloat(t.s)/100,t.l=parseFloat(t.l)/100,0<=t.h&&1>=t.s&&1>=t.l&&(s=c(r(t.h,t.s,t.l)))),void 0!==t.a&&(e=t.a));return(!1!==s||!i)&&(!1===s&&(s="000000"),"#"!=s.charAt(0)?s="#"+(s=s.substr(0,6)):s=s.substr(0,7),{h:s,a:e})}function o(){if(et.quad)return et.quad;var t={};return t.size,t.padding=2,t.path,t.imageData=null,t.dotToSv=function(t){return{s:Math.abs(this.path[3].x-t.x)/this.size,v:Math.abs(this.path[3].y-t.y)/this.size}},t.svToDot=function(t){var i=this.path[0].x,e=this.path[0].y,s=.02;150>V?s=.07:100>V&&(s=.16);for(var o=0;o<this.size;o++)for(var a=0;a<this.size;a++){var h={x:a+i,y:o+e},n=this.dotToSv(h),r=Math.abs(n.v-t.v);if(Math.abs(n.s-t.s)<s&&r<s)return h}return{x:0,y:0}},t.limitDotPosition=function(t){var i=t.x;return t=t.y,i<this.path[0].x&&(i=this.path[0].x),i>this.path[0].x+this.size&&(i=this.path[0].x+this.size),t<this.path[0].y&&(t=this.path[0].y),t>this.path[0].y+this.size&&(t=this.path[0].y+this.size),{x:i,y:t}},t.draw=function(){this.imageData||(this.imageData=W.createImageData(this.size,this.size));for(var t=0,i=this.path[0].x,e=this.path[0].y,s=0;s<this.size;s++)for(var o=0;o<this.size;o++){var a=this.dotToSv({x:o+i,y:s+e});a=r(k.h,a.s,a.v);this.imageData.data[t+0]=a.r,this.imageData.data[t+1]=a.g,this.imageData.data[t+2]=a.b,this.imageData.data[t+3]=255,t+=4}for(W.putImageData(this.imageData,i,e),W.beginPath(),W.strokeStyle="rgba(0,0,0, 0.2)",W.lineWidth=2,t=0;t<=this.path.length-1;++t)0==t?W.moveTo(this.path[t].x,this.path[t].y):W.lineTo(this.path[t].x,this.path[t].y);W.stroke(),W.closePath()},t.updateSize=function(){this.size=Math.floor((2*st.innerRadius-2*ot.paddingX-2*this.padding)/Math.sqrt(2)),this.path=[],this.path[0]={x:this.size/2*-1,y:this.size/2*-1},this.path[1]={x:this.path[0].x+this.size,y:this.path[0].y},this.path[2]={x:this.path[1].x,y:this.path[1].y+this.size},this.path[3]={x:this.path[2].x-this.size,y:this.path[2].y},this.path[4]={x:this.path[0].x,y:this.path[0].y};for(var t=0;t<=this.path.length-1;++t)this.path[t].x+=st.pos.x,this.path[t].y+=st.pos.y},t.isDotIn=function(t){return!(t.x<this.path[0].x||t.x>this.path[0].x+this.size||t.y<this.path[0].y||t.y>this.path[0].y+this.size)},et.quad=t}function a(){if(et.triangle)return et.triangle;var t={};return t.size,t.padding=2,t.path,t.imageData=null,t.followWheel=!0,t.s,t.sOnTop=!1,t.outerRadius,t.limitDotPosition=function(t){var i=t.x;t=t.y;var e,s=this.path[2].x,o=t;e=Math.min(Math.max(s,i),this.path[0].x);var a=(this.path[0].y-this.path[1].y)/(this.path[0].x-this.path[1].x),h=Math.ceil(this.path[1].y+a*(e-this.path[1].x));a=(this.path[0].y-this.path[2].y)/(this.path[0].x-this.path[2].x),a=Math.floor(this.path[2].y+a*(e-this.path[2].x));return i<s&&(o=t),{x:e,y:o=Math.min(Math.max(h,o),a)}},t.svToDot=function(t){var i=.02;150>V?i=.07:100>V&&(i=.16);for(var e=0;e<this.size;e++)for(var s=0;s<this.size;s++){var o={x:this.path[1].x+s,y:this.path[1].y+e};if(S.isDotIn(o)){var a=this.dotToSv(o),h=Math.abs(a.v-t.v);if(Math.abs(a.s-t.s)<i&&h<i)return o}}return{x:0,y:0}},t.draw=function(){this.imageData||(this.imageData=q.createImageData(this.size,this.size)),A.width=this.size,A.height=this.size;for(var t=this.path[1].x,i=this.path[1].y,e=0,s=0;s<this.size;s++)for(var o=0;o<this.size;o++){var a={x:this.path[1].x+o,y:this.path[1].y+s};S.isDotIn(a)?(a=this.dotToSv(a),a=r(k.h,a.s,a.v),this.imageData.data[e+0]=a.r,this.imageData.data[e+1]=a.g,this.imageData.data[e+2]=a.b,this.imageData.data[e+3]=255):(this.imageData.data[e+0]=0,this.imageData.data[e+1]=0,this.imageData.data[e+2]=0,this.imageData.data[e+3]=0),e+=4}for(q.putImageData(this.imageData,0,0),W.drawImage(A,t,i),W.beginPath(),W.strokeStyle="rgba(0, 0, 0, 0.3)",W.lineWidth=2,t=this.path,e=0;e<=t.length-1;++e)0==e?W.moveTo(t[e].x,t[e].y):W.lineTo(t[e].x,t[e].y);W.stroke(),W.closePath()},t.calcS=function(t){return Math.abs((t[1].x-t[0].x)*(t[2].y-t[0].y)-(t[2].x-t[0].x)*(t[1].y-t[0].y))/2},t.dotToSv=function(t){var i=this.vol,e=((t.x-i[0].x)*(i[1].x-i[0].x)+(t.y-i[0].y)*(i[1].y-i[0].y))/((i[0].x-i[1].x)*(i[0].x-i[1].x)+(i[0].y-i[1].y)*(i[0].y-i[1].y));0>e&&(e=0),1<e&&(e=1);var s=this.vol[0];return 1>(i=Math.sqrt(Math.pow(i[0].x+e*(i[1].x-i[0].x)-s.x,2)+Math.pow(i[0].y+e*(i[1].y-i[0].y)-s.y,2)))&&(i=Math.floor(i)),i>this.h-1&&(i=this.h),i/=this.h,30>(t=Math.abs(d(t,this.sSide)))&&(t=30),{s:(60-(t-30))/60,v:i}},t.isDotIn=function(t){t=[{x:this.path[0].x,y:this.path[0].y},{x:this.path[1].x,y:this.path[1].y},{x:t.x,y:t.y}];var i=this.calcS(t);return t[1]={x:this.path[2].x,y:this.path[2].y},i+=this.calcS(t),t[0]={x:this.path[1].x,y:this.path[1].y},i+=this.calcS(t),Math.ceil(i)==Math.ceil(this.s)},t.updateSize=function(){this.outerRadius=st.innerRadius-ot.paddingX-this.padding,this.size=Math.floor(2*this.outerRadius*Math.sin(P/180*60));var t=Math.sqrt(3)/2*this.size;for(this.h=Math.sqrt(3)/2*this.size,this.path=[],this.path[0]={x:this.outerRadius,y:0},this.path[1]={x:this.path[0].x-t,y:this.size/2*-1},this.path[2]={x:this.path[1].x,y:this.size/2},this.path[3]={x:this.path[0].x,y:this.path[0].y},t=0;t<=this.path.length-1;++t)this.path[t].x+=st.pos.x,this.path[t].y+=st.pos.y;this.vol=[],this.s=this.calcS(this.path),this.sOnTop?(t=p(this.path[0],this.path[2]),this.vol[0]={x:this.path[1].x,y:this.path[1].y},this.vol[1]={x:t.x,y:t.y},this.sSide=this.path[1]):(t=p(this.path[0],this.path[1]),this.vol[0]={x:this.path[2].x,y:this.path[2].y},this.vol[1]={x:t.x,y:t.y},this.sSide=this.path[2])},et.triangle=t}function h(t,i,e,s){return"object"!=typeof t&&(t=document.getElementById(t)),!!t&&(s||(s=""),U[s+i]=e,t.addEventListener?t.addEventListener(i,U[s+i]):t.attachEvent("on"+i,U[s+i]),!0)}function n(t,i,e){return"object"!=typeof t&&(t=document.getElementById(t)),!!t&&(e||(e=""),!!U[e+i]&&(t.removeEventListener?t.removeEventListener(i,U[e+i]):t.detachEvent("on"+i,U[e+i]),U[e+i]=null,!0))}function r(t,i,e){var s,o,a,h,n,r;switch(t&&void 0===i&&void 0===e&&(i=t.s,e=t.v,t=t.h),n=6*t-(h=Math.floor(6*t)),t=e*(1-i),r=e*(1-n*i),i=e*(1-(1-n)*i),h%6){case 0:s=e,o=i,a=t;break;case 1:s=r,o=e,a=t;break;case 2:s=t,o=e,a=i;break;case 3:s=t,o=r,a=e;break;case 4:s=i,o=t,a=e;break;case 5:s=e,o=t,a=r}return{r:Math.floor(255*s),g:Math.floor(255*o),b:Math.floor(255*a)}}function l(t,i,e){t&&void 0===i&&void 0===e&&(i=t.g,e=t.b,t=t.r),t/=255,i/=255,e/=255;var s,o=Math.max(t,i,e),a=Math.min(t,i,e),h=o-a;if(o==a)s=0;else{switch(o){case t:s=(i-e)/h+(i<e?6:0);break;case i:s=(e-t)/h+2;break;case e:s=(t-i)/h+4}s/=6}return{h:s,s:0==o?0:h/o,v:o}}function u(t){return{r:(t=parseInt("#"==t.charAt(0)?t.slice(1):t,16))>>16,g:t>>8&255,b:255&t}}function c(t){var i=function(t){return 1===(t=t.toString(16)).length?"0"+t:t};return"#"+i(t.r)+i(t.g)+i(t.b)}function p(t,i){return{x:(t.x+i.x)/2,y:(t.y+i.y)/2}}function d(t,i,e){return i||(i={x:0,y:0}),t=180*Math.atan2(t.y-i.y,t.x-i.x)/P,e&&0>t&&(t=360+t),t}function g(){z=2+ot.paddingX,L=!1,st.imageData=null,D=V/2,st.pos={x:D,y:D},st.outerRadius=D-z,st.innerRadius=st.outerRadius-st.width,ot.path=[{x:st.innerRadius-ot.paddingX,y:-1*ot.height},{x:st.outerRadius+ot.paddingX,y:-1*ot.height},{x:st.outerRadius+ot.paddingX,y:ot.height},{x:st.innerRadius-ot.paddingX,y:ot.height},{x:st.innerRadius-ot.paddingX,y:-1*(ot.height+ot.lineWeight/2)}];var t=V;for(R&&(t+=at.width+2*at.padding),"canvas"!=j.tagName.toLowerCase()&&(j.style.width=t+"px",j.style.height=V+"px"),I.width=t,I.height=V,Z!=I&&(I.style.width=t+"px",I.style.height=V+"px"),t=0;t<=tt.length-1;++t)tt[t].updateSize();it&&(it.imageData.triangle=null,it.imageData.quad=null,it.updateSize()),S.updateSize(),R&&at.updateSize()}function y(t){if(H&&(!F.updateinput||(0,F.updateinput)(G,H,t))){var i=Q.toFixed(2),e="rgba("+_.r+", "+_.g+", "+_.b+", "+i+")";if(!t)switch(O){case"mixed":H.value=1>Q?e:J;break;case"hex":H.value=J;break;case"hsla":H.value="hsla("+(360*k.h).toFixed(2)+", "+(100*k.s).toFixed(2)+"%, "+(100*k.v).toFixed(2)+"%, "+i+")";break;default:H.value=e}N&&(H.style.color=.5>k.v?"#FFF":"#000",H.style.background=e)}}function v(){h(I,"mousedown",function(t){G.mouseDownEvent(t)},"wait_action_"),h(I,"touchstart",function(t){G.mouseDownEvent(t)},"wait_action_"),h(I,"mouseout",function(t){G.mouseOutEvent(t)},"wait_action_"),h(window,"touchmove",function(t){G.touchMoveEvent(t)},"wait_action_"),h(I,"mousemove",function(t){G.mouseMoveRest(t)},"wait_action_")}function f(){n(I,"mousedown","wait_action_"),n(I,"touchstart","wait_action_"),n(I,"mouseout","wait_action_"),n(window,"touchmove","wait_action_"),n(I,"mousemove","wait_action_")}function m(t){t=t||window.event;var i,e=document.body.scrollLeft+document.documentElement.scrollLeft,s=document.body.scrollTop+document.documentElement.scrollTop;"touchend"==t.type?(i=t.changedTouches[0].clientX+e,t=t.changedTouches[0].clientY+s):"touchmove"==t.type||t.touches?(i=t.touches[0].clientX+e,t=t.touches[0].clientY+s):(i=t.clientX+e,t=t.clientY+s);var o=I.getBoundingClientRect();return{x:i-=o.left+e,y:t-=o.top+s}}function x(t){for(var i=!1,e=0;e<=tt.length-1;++e)tt[e].selected&&(i=e),tt[e].selected=!1;var s=!1;for(e=0;e<=tt.length-1;++e)if(e==t){tt[e].selected=!0,G.setColorByHex(tt[e].color),s=!0;break}return s&&F.selectcolorsaver&&(0,F.selectcolorsaver)(G,tt[t]),s||!1===i||(tt[i].selected=!0),s}function w(){for(var t=0;t<=tt.length-1;++t)tt[t].selected&&(tt[t].color=J)}function b(){if(tt.length)for(var t=0;t<=tt.length-1;++t)tt[t].draw()}function C(){if(!(W&&(W.clearRect(0,0,I.width,I.height),L?(W.putImageData(X,0,0),b(),1):(st.draw(),S.draw(),R&&at.draw(),b(),it&&it.draw(),B||(X=W.getImageData(0,0,I.width,I.height),L=!0),1))))return!1;var t=360*k.h-st.startAngle;R&&(W.beginPath(),W.rect(at.pos.x-2,at.padding+at.height*(1-Q)-1,at.width+4,2),W.strokeStyle="rgba(0,0,0, 0.8)",W.lineWidth=2,W.stroke(),W.closePath()),W.beginPath();var i,e=ot.path;i=P/180*t;t=[];for(var s=0;s<=e.length-1;++s)t[s]={x:e[s].x*Math.cos(i)-e[s].y*Math.sin(i),y:e[s].x*Math.sin(i)+e[s].y*Math.cos(i)};for(e=0;e<=t.length-1;++e)t[e].x+=st.pos.x,t[e].y+=st.pos.y,0==e?W.moveTo(t[e].x,t[e].y):W.lineTo(t[e].x,t[e].y);return W.strokeStyle="rgba(0,0,0,0.8)",W.lineWidth=ot.lineWeight,W.stroke(),W.closePath(),W.strokeStyle=.5<k.v&&.5>k.s?"rgba(0, 0, 0, 1)":"rgba(255, 255, 255, 1)",W.beginPath(),W.lineWidth=2,W.arc(k.x,k.y,T.radius,0,2*P),W.stroke(),W.closePath(),!1}var S,z,D,k,_,P=Math.PI,M=!0,T={radius:4},I=!1,W=!1,E="quad",R=!1,B=!1,K=!0,U=[],F=[],A=document.createElement("canvas"),q=!1,L=!1,X=null,H=!1,N=!0,O="mixed",Y={tag:!1,margin:6},j=!1,G=this,V=200,J="#000000",Q=1,Z=!1,$=!1,tt=[],it=!1,et=[],st={width:18,imageData:null};st.innerRadius,st.startAngle=0,st.outerRadius,st.outerStrokeStyle="rgba(0,0,0,0.2)",st.innerStrokeStyle="rgba(0,0,0,0.2)",st.pos,st.draw=function(){if(this.imageData)W.putImageData(this.imageData,0,0);else{for(var t=this.startAngle,i=0;360>=i;i++){var e=P/180*(i-2),s=P/180*i;W.beginPath(),W.moveTo(D,D),W.arc(D,D,this.outerRadius,e,s,!1),W.closePath(),e=r(t/360,1,1),W.fillStyle="rgb("+e.r+", "+e.g+", "+e.b+")",W.fill(),360<=++t&&(t=0)}W.globalCompositeOperation="destination-out",W.beginPath(),W.arc(D,D,this.innerRadius,0,2*P),W.fill(),W.globalCompositeOperation="source-over",W.strokeStyle=this.innerStrokeStyle,W.lineWidth=2,W.stroke(),W.closePath(),W.beginPath(),W.arc(D,D,this.outerRadius,0,2*P),W.strokeStyle=this.outerStrokeStyle,W.lineWidth=2,W.stroke(),W.closePath(),this.imageData=W.getImageData(0,0,V,V)}},st.isDotIn=function(t){return Math.pow(this.pos.x-t.x,2)+Math.pow(this.pos.y-t.y,2)<Math.pow(this.outerRadius,2)&&Math.pow(this.pos.x-t.x,2)+Math.pow(this.pos.y-t.y,2)>Math.pow(this.innerRadius,2)};var ot={lineWeight:2,height:4,paddingX:2};ot.path;var at={width:18,padding:4,outerStrokeStyle:"rgba(0,0,0,0.2)",innerStrokeStyle:"rgba(0,0,0,0.2)"};at.height,at.pos,at.updateSize=function(){this.pos={x:V+at.padding,y:at.padding},this.height=V-2*at.padding},at.draw=function(){var t=W.createLinearGradient(0,0,0,this.height),i=r(k.h,1,1);t.addColorStop(0,"rgba("+i.r+","+i.g+","+i.b+",1)"),t.addColorStop(1,"rgba("+i.r+","+i.g+","+i.b+",0)"),W.beginPath(),W.rect(this.pos.x,this.pos.y,this.width,this.height),W.fillStyle="white",W.fill(),W.fillStyle=t,W.fill(),W.strokeStyle="rgba(0,0,0, 0.2)",W.lineWidth=2,W.stroke(),W.closePath()},at.dotToAlpha=function(t){return 1-Math.abs(this.pos.y-t.y)/this.height},at.alphaToDot=function(t){return{x:0,y:this.height-this.height*t}},at.limitDotPosition=function(t){return(t=t.y)<this.pos.y&&(t=this.pos.y),t>this.pos.y+this.height&&(t=this.pos.y+this.height),{x:this.pos.x,y:t}},at.isDotIn=function(t){return!(t.x<this.pos.x||t.x>this.pos.x+at.width||t.y<this.pos.y||t.y>this.pos.y+this.height)};var ht,nt,rt,lt={svCursorData:null,stCursor:null,curType:0,size:16};lt.cEl=document.body,lt.initSvCursor=function(){if(!I)return!1;if(this.curType=1,this.stCursor||(this.stCursor=window.getComputedStyle(this.cEl).cursor,this.stCursor||(this.stCursor="auto")),this.svCursorData)return this.cEl.style.cursor=this.svCursorData,!0;if(!A)return!1;var t=this.size+2;A.width=t,A.height=t,q.clearRect(0,0,this.size,this.size),q.strokeStyle="rgba(255, 255, 255, 1)",q.beginPath(),q.lineWidth=2,q.arc(t/2,t/2,this.size/2,0,2*P),q.stroke(),q.closePath();var i=A.toDataURL();return this.svCursorData="url("+i+") "+t/2+" "+t/2+", auto",!!this.svCursorData&&(this.cEl.style.cursor=this.svCursorData,-1===this.cEl.style.cursor.indexOf(i)&&(this.svCursorData="crosshair",this.cEl.style.cursor="crosshair"),!0)},lt.initStandartCursor=function(){this.stCursor&&(lt.curType=0,this.cEl.style.cursor=this.stCursor)},lt.updateCursor=function(t){M&&(KellyColorPicker.cursorLock||(S.isDotIn(t)?lt.initSvCursor():lt.initStandartCursor()))},this.popUpClose=function(t){if(!1!==Y.tag){if(t&&(t.target==H||t.target==I||t.target==Y.tag))return!1;F.popupclose&&!F.popupclose(G,t)||(Y.tag.style.display="none",KellyColorPicker.activePopUp==G&&(KellyColorPicker.activePopUp=!1))}},this.popUpShow=function(t){if(!1!==Y.tag&&(!F.popupshow||F.popupshow(G,t))){KellyColorPicker.popupEventsInclude||(h(document,"click",function(t){return!!KellyColorPicker.activePopUp&&KellyColorPicker.activePopUp.popUpClose(t)},"popup_close_"),h(window,"resize",function(t){if(KellyColorPicker.activePopUp)return KellyColorPicker.activePopUp.popUpShow(t)},"popup_resize_"),KellyColorPicker.popupEventsInclude=!0),KellyColorPicker.activePopUp&&KellyColorPicker.activePopUp.popUpClose(!1),t=G.getCanvas().width;var i=G.getAlphaFig();i&&(t-=i.width+i.padding),i=window.getComputedStyle(Y.tag),0>=(i=parseInt(i.paddingBottom)+parseInt(i.paddingTop))&&(i=0);var e=H.getBoundingClientRect(),s=e.left+(window.scrollX||window.pageXOffset||document.body.scrollLeft);return Y.tag.style.top=e.top+(window.scrollY||window.pageYOffset||document.body.scrollTop)-i-t-Y.margin+"px",Y.tag.style.left=s+"px",Y.tag.style.display="block",KellyColorPicker.activePopUp=G,!1}},this.setHueByDot=function(t){0>(t=d(t,st.pos)+st.startAngle)&&(t=360+t),k.h=t/360,_=r(k.h,k.s,k.v),J=c(_),w(),F.change&&(0,F.change)(G),y(),L=!1,C()},this.setColorForColorSaver=function(t,i){var e=s(t,!0);if(e){var o=G.getColorSaver(i);return o.selected?this.setColorByHex(t,!1):(o.color=e.h,C()),!0}},this.setColor=function(t,i){G.setColorByHex(t,i)},this.setColorByHex=function(t,i){i||(i=!1);var e=Q;if(!1!==t){if(!t||!t.length)return;var o=s(t,!0);if(!o)return;t=o.h,R&&(e=o.a)}else t=J;R&&t==J&&L&&e!=Q?(Q=e,C()):J&&t==J&&L||(Q=e,_=u(t),J=t,k=l(_),e=S.svToDot(k),k.x=e.x,k.y=e.y,L=!1,w(),C(),F.change&&(0,F.change)(G),y(i))},this.setAlphaByDot=function(t){Q=at.dotToAlpha(t),F.change&&(0,F.change)(G),y(),C()},this.setAlpha=function(t){Q=t,y(),C()},this.setColorByDot=function(t){var i=S.dotToSv(t);k.s=i.s,k.v=i.v,k.x=t.x,k.y=t.y,1<k.s&&(k.s=1),0>k.s&&(k.s=0),1<k.v&&(k.v=1),0>k.v&&(k.v=0),_=r(k.h,k.s,k.v),J=c(_),w(),F.change&&(0,F.change)(G),y(),C()},this.mouseOutEvent=function(t){0<lt.curType&&!KellyColorPicker.cursorLock&&lt.initStandartCursor()},this.mouseMoveRest=function(t){if(!B&&K){K=!1;var i=m(t);lt.updateCursor(i),requestAnimationFrame(function(){K=!0}),F.mousemoverest&&(0,F.mousemoverest)(t,G,i)}},this.touchMoveEvent=function(t){B&&event.preventDefault()},this.mouseDownEvent=function(t){t.preventDefault();var i,e=!1,s=m(t);if(st.isDotIn(s))B="wheel",G.setHueByDot(s),i=function(t){G.wheelMouseMove(t,s)},e=function(t){KellyColorPicker.cursorLock=!1,G.wheelMouseUp(t,s)};else if(S.isDotIn(s))B="sv",G.setColorByDot(s),i=function(t){G.svMouseMove(t,s)},e=function(t){KellyColorPicker.cursorLock=!1,G.svMouseUp(t,s)};else if(R&&at.isDotIn(s))B="alpha",G.setAlphaByDot(s),i=function(t){G.alphaMouseMove(t,s)},e=function(t){KellyColorPicker.cursorLock=!1,G.alphaMouseUp(t,s)};else if(it&&it.isDotIn(s))G.setMethod();else if(tt.length)for(t=0;t<=tt.length-1;++t)if(tt[t].isDotIn(s)){x(t);break}i&&e&&(f(),KellyColorPicker.cursorLock=G,h(document,"mouseup",e,"action_process_"),h(document,"mousemove",i,"action_process_"),h(document,"touchend",e,"action_process_"),h(document,"touchmove",i,"action_process_"))},this.wheelMouseMove=function(t,i){if(t.preventDefault(),B&&K){K=!1;var e=m(t);requestAnimationFrame(function(){K=!0}),G.setHueByDot(e),F.mousemoveh&&(0,F.mousemoveh)(t,G,e)}},this.wheelMouseUp=function(t,i){if(t.preventDefault(),B){n(document,"mouseup","action_process_"),n(document,"mousemove","action_process_"),n(document,"touchend","action_process_"),n(document,"touchmove","action_process_"),v(),L=B=!1,C();var e=m(t);lt.updateCursor(e),F.mouseuph&&(0,F.mouseuph)(t,G,e)}},this.alphaMouseMove=function(t,i){if(t.preventDefault(),B&&K){K=!1;var e=m(t);e=at.limitDotPosition(e);requestAnimationFrame(function(){K=!0}),G.setAlphaByDot(e),F.mousemovealpha&&(0,F.mousemovealpha)(t,G,e)}},this.alphaMouseUp=function(t,i){if(t.preventDefault(),B){n(document,"mouseup","action_process_"),n(document,"mousemove","action_process_"),n(document,"touchend","action_process_"),n(document,"touchmove","action_process_"),v(),B=!1;var e=m(t);lt.updateCursor(e),F.mouseupalpha&&(0,F.mouseupalpha)(t,G,e)}},this.svMouseMove=function(t,i){if(t.preventDefault(),B&&K){K=!1;var e=m(t);e=S.limitDotPosition(e);requestAnimationFrame(function(){K=!0}),G.setColorByDot(e),F.mousemovesv&&(0,F.mousemovesv)(t,G,e)}},this.svMouseUp=function(t,i){if(t.preventDefault(),B){n(document,"mouseup","action_process_"),n(document,"mousemove","action_process_"),n(document,"touchend","action_process_"),n(document,"touchmove","action_process_"),v(),B=!1;var e=m(t);lt.updateCursor(e),R&&(L=!1,C()),F.mouseupsv&&(0,F.mouseupsv)(t,G,e)}},this.addUserEvent=function(t,i){return F[t]=i,!0},this.removeUserEvent=function(t){return!!F[t]&&(F[t]=null,!0)},this.getCanvas=function(){return!!W&&I},this.getCtx=function(){return W||!1},this.getInput=function(){return H},this.getSvFig=function(){return S},this.getSvFigCursor=function(){return T},this.getWheel=function(){return st},this.getWheelCursor=function(){return ot},this.getCurColorHsv=function(){return k},this.getCurColorRgb=function(){return _},this.getCurColorHex=function(){return J},this.getCurColorRgba=function(){return{r:_.r,g:_.g,b:_.b,a:Q}},this.getCurAlpha=function(){return Q},this.getAlphaFig=function(){return!!R&&at},this.getPopup=function(){return Y},this.getSize=function(){return V},this.getColorSaver=function(t){for(var i=0;i<=tt.length-1;++i)if(!t&&tt[i].selected||tt[i].align==t)return tt[i].rgb=u(tt[i].color),tt[i].hsv=l(tt[i].rgb.r,tt[i].rgb.g,tt[i].rgb.b),tt[i]},this.setColorSaver=function(t){if(!t)return!1;for(var i=0;i<=tt.length-1;++i)if(tt[i].align==t)return x(i),tt[i]},this.updateView=function(t){return!!W&&(t&&(st.imageData=null,X=S.imageData=null),L=!1,g(),C(),!0)},this.resize=function(t,i){return!!W&&(t==V&&!i||(L=!1,st.imageData=null,X=S.imageData=null,V=t,g(),G.setColorByHex(!1),!1))},this.syncSize=function(t){return!!Z&&((t=e(Z))&&G.resize(t),!1)},this.setMethod=function(t){return t||(t="triangle","triangle"==E&&(t="quad")),t!=E&&("quad"==E||"triangle"==E)&&("quad"==(E=t)&&(S=o()),"triangle"==E&&(S=a()),G.resize(V,!0),F.setmethod&&(0,F.setmethod)(G,E),!0)},this.destroy=function(){if(!G)return!1;0<lt.curType&&(KellyColorPicker.cursorLock=!1,lt.initStandartCursor()),B&&(n(document,"mouseup","action_process_"),n(document,"mousemove","action_process_"),n(document,"touchend","action_process_"),n(document,"touchmove","action_process_"),B=!1),Y.tag&&n(H,"click","popup_"),H&&(n(H,"click","input_edit_"),n(H,"change","input_edit_"),n(H,"keyup","input_edit_"),n(H,"keypress","input_edit_")),KellyColorPicker.popupEventsInclude&&U.popup_close_click&&(KellyColorPicker.activePopUp&&KellyColorPicker.activePopUp.popUpClose(!1),n(document,"click","popup_close_"),n(window,"resize","popup_resize_"),KellyColorPicker.popupEventsInclude=!1),st.imageData=null,A=X=S.imageData=null,j&&j.parentNode&&j.parentNode.removeChild(j),Z&&n(window,"resize","canvas_"),f(),G=null},nt="",rt="",void 0!==(ht=t).alpha_slider&&(ht.alphaSlider=ht.alpha_slider),void 0!==ht.input_color&&(ht.inputColor=ht.input_color),void 0!==ht.input_format&&(ht.inputFormat=ht.input_format),ht.input&&"object"!=typeof ht.input?(ht.input=document.getElementById(ht.input),H=ht.input):"object"==typeof ht.input&&(H=ht.input),void 0!==ht.changeCursor&&(M=ht.changeCursor),void 0!==ht.alpha&&(Q=ht.alpha),void 0!==ht.alphaSlider&&(R=ht.alphaSlider),void 0!==ht.inputColor&&(N=ht.inputColor),void 0!==ht.inputFormat&&(O=ht.inputFormat),ht.userEvents&&(F=ht.userEvents),ht.place&&"object"!=typeof ht.place&&(rt=ht.place,ht.place=document.getElementById(ht.place)),ht.place?j=ht.place:H?(Y.tag=document.createElement("div"),Y.tag.className="popup-kelly-color",ht.popupClass?Y.tag.className=ht.popupClass:(Y.tag.className="popup-kelly-color",Y.tag.style.position="absolute",Y.tag.style.bottom="0px",Y.tag.style.left="0px",Y.tag.style.display="none",Y.tag.style.backgroundColor="#e1e1e1",Y.tag.style.border="1px solid #bfbfbf",Y.tag.style.boxShadow="7px 7px 14px -3px rgba(0,0,0,0.24)",Y.tag.style.borderTopLeftRadius="4px",Y.tag.style.borderTopRightRadius="4px",Y.tag.style.borderBottomLeftRadius="4px",Y.tag.style.borderBottomRightRadius="4px",Y.tag.style.padding="12px",Y.tag.style.boxSizing="content-box"),j=Y.tag,document.getElementsByTagName("body")[0].appendChild(Y.tag),h(H,"click",function(t){return G.popUpShow(t)},"popup_")):nt+='| "place" ('+rt+") not not found",rt=!1,ht.color?rt=s(ht.color):H&&H.value&&(rt=s(H.value)),rt&&(J=rt.h,R&&(Q=rt.a)),!ht.method||"triangle"!=ht.method&&"quad"!=ht.method||(E=ht.method),j&&("CANVAS"!=j.tagName?(I=document.createElement("CANVAS"),j.appendChild(I)):I=j,void 0!==window.G_vmlCanvasManager&&(I=window.G_vmlCanvasManager.initElement(I),A=window.G_vmlCanvasManager.initElement(A)),I.getContext&&I.getContext("2d")&&(W=I.getContext("2d"),q=A.getContext("2d"),1))||(nt+=" | cant init canvas context"),ht.resizeWith&&("object"!=typeof ht.resizeWith&&"boolean"!=typeof ht.resizeWith&&(ht.resizeWith=document.getElementById(ht.resizeWith)),Z=!0===ht.resizeWith?I:ht.resizeWith,ht.resizeSide&&($=ht.resizeSide),Z&&(e(Z)&&(ht.size=e(Z)),h(window,"resize",function(t){return G.syncSize(t)},"canvas_"))),ht.size&&0<ht.size&&(V=ht.size),nt?"undefined"!=typeof console&&console.log("KellyColorPicker : "+nt):("quad"==E&&(S=o()),"triangle"==E&&(S=a()),H&&(h(H,"click",nt=function(t){(t=t||window.event).target||(t.target=t.srcElement),G.setColorByHex(t.target.value,!0)},"input_edit_"),h(H,"change",nt,"input_edit_"),h(H,"keyup",nt,"input_edit_"),h(H,"keypress",nt,"input_edit_")),ht.colorSaver&&(i("left",!0),i("right")),ht.methodSwitch&&((it={}).sizePercentage=10,it.position,it.paddingY=4,it.paddingX=4,it.imageData=[],it.lineWidth=2,it.color="#c1ebf5",it.updateSize=function(){this.size=parseInt(V-V/100*(100-this.sizePercentage)),16>this.size&&(this.size=16),this.position={x:this.paddingX,y:this.paddingY}},it.draw=function(){var t,i;if(this.imageData[E])W.putImageData(this.imageData[E],this.position.x,this.position.y);else{var e=u(this.color);A.width=this.size,A.height=this.size,q.clearRect(0,0,this.size,this.size),q.beginPath();var s="triangle";if("triangle"==E&&(s="quad"),q.beginPath(),35>this.size?i=t=A.width/2:(t=A.width/2-this.lineWidth,q.arc(this.size/2,this.size/2,t,0,2*P),q.strokeStyle="rgba(0, 0, 0, 0.4)",q.lineWidth=this.lineWidth,q.stroke(),i=t-6,q.closePath(),q.beginPath(),q.arc(this.size/2,this.size/2,i,0,2*P),q.strokeStyle="rgba(0, 0, 0, 0.4)",q.lineWidth=this.lineWidth,q.stroke(),q.closePath()),q.beginPath(),"quad"==s)s=Math.floor((2*i-4)/Math.sqrt(2)),t=(i=(this.size-s)/2)+s,i=i+s/2-s/2,q.moveTo(t,i),q.lineTo(t-s,i),q.lineTo(t-s,i+s),q.lineTo(t,i+s);else{s=Math.floor((2*i-4)*Math.sin(P/180*60)),t=2*i+(t-i),i=this.size/2;var o=Math.sqrt(3)/2*s;q.moveTo(t,i),q.lineTo(t-o,i-s/2),q.lineTo(t-o,i+s/2),q.lineTo(t,i)}q.lineTo(t,i),q.fillStyle="rgba("+e.r+","+e.g+","+e.b+", 1)",q.fill(),q.lineWidth=this.lineWidth,q.strokeStyle="rgba(0, 0, 0, 0.6)",q.stroke(),q.closePath(),this.imageData[E]=q.getImageData(0,0,A.width,A.width),W.drawImage(A,this.position.x,this.position.y)}},it.isDotIn=function(t){return t.x>=this.position.x&&t.x<=this.position.x+this.size&&t.y>=this.position.y&&t.y<=this.position.y+this.size}),v(),g(),G.setColorByHex(!1))}KellyColorPicker.cursorLock=!1,KellyColorPicker.activePopUp=!1,KellyColorPicker.popupEventsInclude=!1,KellyColorPicker.attachToInputByClass=function(t,i){for(var e=[],s=document.getElementsByClassName(t),o=0;o<s.length;o++)i?i.input=s[o]:i={input:s[o],size:150},e.push(new KellyColorPicker(i));return e};
);

void handleMilligram() {
  server.send(200, "text/css", FPSTR(milligram));
}

void handleKelly() {
  server.send(200, "text/javascript", FPSTR(kellypicker));
}

void handleRoot() {
  digitalWrite(led, 1);

  server.send(200, "text/html", FPSTR(postForms));

  digitalWrite(led, 0);
}

void handleAjax() {
  digitalWrite(led, 1);

  if (server.method() == HTTP_POST) {
   Serial1.print(server.arg("command") + "\n");
   server.send(200, "text/html", server.arg("command") + "\n");
  }

  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial1.begin(9600);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led, 1);
    delay(50);
    digitalWrite(led, 0);
    delay(50);
    digitalWrite(led, 1);
    delay(50);
    digitalWrite(led, 0);
    delay(500);
  }

  if(MDNS.begin("esp8266")) {
  }

  server.on("/", handleRoot);

  server.on("/submit", handleAjax);

  // css files
  server.on("/milligram.min.css", handleMilligram);

  // javascript files
  server.on("/kelly.js", handleKelly);

  server.onNotFound(handleNotFound);

  server.begin();
}

void loop(void) {
  server.handleClient();
  // Discard byte in the read buffer if present
  Serial1.read();
}
