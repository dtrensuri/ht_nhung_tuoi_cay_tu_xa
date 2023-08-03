#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <time.h>
#include <SerialCommand.h>
#include <SoftwareSerial.h>
#define PORT_NUMBER 8188

SerialCommand SCmd;

ESP8266WebServer webServer(PORT_NUMBER);

String timeOn,timeOff;

void setup() {
  Serial.begin(115200);
  configESP8266();
  webServer.begin();
  pinMode(D1, INPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  webServer.on("/", mainpage);
  webServer.on("/onD1", on_D1);
  webServer.on("/offD1", off_D1);
  webServer.on("/onD2", on_D2);
  webServer.on("/offD2", off_D2);
  webServer.on("/onD3", on_D3);
  webServer.on("/offD3", off_D3);
  webServer.on("/getstatus", get_status);
  webServer.on("/setTimeRTC", setTimeRTC);
  webServer.begin();
}

void loop() {
  webServer.handleClient();
}

void configESP8266() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("TuoiCayWifi", "44448888");
}


const char MainPage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
  <title>HOME PAGE</title>
  <style>
    body {
      display: flex;
      text-align: center;
      justify-content: center;
    }

    h1 {
      color: #003399;
    }

    a {
      text-decoration: none;
      color: #FFFFFF;
    }

    .bt_on {
      height: 50px;
      width: 100px;
      margin: 10px 0;
      background-color: #FF6600;
      border-radius: 5px;
    }

    .bt_off {
      height: 50px;
      width: 100px;
      margin: 10px 0;
      background-color: #00FF00;
      border-radius: 5px;
    }

    #loader {
      border: 16px solid #f3f3f3;
      border-radius: 50%;
      border-top: 16px solid #3498db;
      width: 120px;
      height: 120px;
      display: none;
      -webkit-animation: spin 2s linear infinite;
      /* Safari */
      animation: spin 2s linear infinite;
    }

    /* Safari */
    @-webkit-keyframes spin {
      0% {
        -webkit-transform: rotate(0deg);
      }

      100% {
        -webkit-transform: rotate(360deg);
      }
    }

    @keyframes spin {
      0% {
        transform: rotate(0deg);
      }

      100% {
        transform: rotate(360deg);
      }
    }
  </style>
  <meta charset="UTF-8">
</head>

<body>

  <div id="panel">
    <h1>Web Tuoi Cay</h1>
    <div id="clock"></div>
    <div>Trang thai may bom: <b>
        <pan id="trangthaiD1">
          <pan>
      </b></div>
    <div>
      <button class="bt_on" onclick="getdata('onD1')">ON</button>
      <button class="bt_off" onclick="getdata('offD1')">OFF</button>
    </div>
    <div>Auto tuoi theo do am: <b>
        <pan id="trangthaiD2">
          <pan>
      </b></div>
    <div>
      <button class="bt_on" onclick="getdata('onD2')">ON</button>
      <button class="bt_off" onclick="getdata('offD2')">OFF</button>
    </div>
    <div>Auto tuoi theo thoi gian: <b>
        <pan id="trangthaiD3">
          <pan>
      </b></div>
    <div>
      <button class="bt_on" onclick="getdata('onD3')">ON</button>
      <button class="bt_off" onclick="getdata('offD3')">OFF</button>
    </div>
    <form action="/">
      <label for="timeOn">Time on:</label>
      <input type="time" id="timeOn" name="timeOn">
      <label for="timeOff">Time off:</label>
      <input type="time" id="timeOff" name="timeOff">
      <input type="submit">
    </form>
    <div id="reponsetext"></div>
  </div>
  <div id="loader"></div>
  <script>
    var set_time = false;
    function clock() {
      //Khởi tạo đối tượng timer sử dụng Date Object
      var timer = new Date();
      //Gọi các phương thức của đối tượng timer
      var hour = timer.getHours();  //Lấy giờ hiện tại (giá trị từ 0 - 23)
      var minute = timer.getMinutes();  //Lấy phút hiện tại
      var second = timer.getSeconds();  //Lấy giây  hiện tại
      //Thêm ký tự 0 đằng trước nếu giờ, phút, giây < 10 với câu lệnh điều khiển if
      if (hour < 10) {
        hour = "0" + hour;
      }
      if (minute < 10) {
        minute = "0" + minute;
      }
      if (second < 10) {
        second = "0" + second;
      }
      //Hiện thị thời gian lên thẻ div id="clock" với phương thức innerHTML
      document.getElementById("clock").innerHTML = hour + ":" + minute + ":" + second;
      if (set_time == false) {
        xhttp.open("GET", "/setTimeRTC?time=" + document.getElementById("clock").textContent, true);
        xhttp.send();
        set_time = true;
      }
    }
    //Thực hiện hàm clock theo chu kỳ 1 giây
    setInterval("clock()", 1000);
    //-----------Tạo đối tượng request----------------
    function create_obj() {
      td = navigator.appName;
      if (td == "Microsoft Internet Explorer") {
        obj = new ActiveXObject("Microsoft.XMLHTTP");
      } else {
        obj = new XMLHttpRequest();
      }
      return obj;
    }
    var xhttp = create_obj();
    //-----------Thiết lập dữ liệu và gửi request-------
    function getdata(url) {
      xhttp.open("GET", "/" + url, true);
      xhttp.onreadystatechange = process;//nhận reponse
      document.getElementById("loader").style.display = "block"
      document.getElementById("panel").style.display = "none";
      xhttp.send();

      var delayInMilliseconds = 3000; //1 second
      setTimeout(function () {
        document.getElementById("panel").style.display = "block";
        document.getElementById("loader").style.display = "none"

      }, delayInMilliseconds);

    }
    //-----------Kiểm tra response--------------------
    function process() {
      if (xhttp.readyState == 4 && xhttp.status == 200) {
        //------Updat data sử dụng javascript----------
        ketqua = xhttp.responseText;
        document.getElementById("reponsetext").innerHTML = ketqua;
      }
    }
    //------------Kiểm tra trạng thái chân D1 D2 D3------
    function getstatus() {
      xhttp.open("GET", "getstatus", true);
      xhttp.onreadystatechange = process_json;
      xhttp.send();
    }
    //-----------Kiểm tra response-------------------------------
    var gTime = false;
    function process_json() {
      if (xhttp.readyState == 4 && xhttp.status == 200) {
        //------Update data sử dụng javascript-------------------
        var trangthai_JSON = xhttp.responseText;
        var Obj = JSON.parse(trangthai_JSON);
        document.getElementById("trangthaiD1").innerHTML = Obj.D1;
        document.getElementById("trangthaiD2").innerHTML = Obj.D2;
        document.getElementById("trangthaiD3").innerHTML = Obj.D3;
        if (gTime == false) {
          document.getElementById("timeOn").value = Obj.timeOn;
          document.getElementById("timeOff").value = Obj.timeOff;
          gTime = true;
        }
      }
    }
    //---Ham update duu lieu tu dong---
    setInterval(function () {
      getstatus();
    }, 1000);
  </script>
</body>

</html>
)=====";

void mainpage(){
   timeOn = webServer.arg("timeOn");
   timeOff = webServer.arg("timeOff");
   Serial.print("TIME_ON "+timeOn+"\r\n");
   Serial.print("TIME_OFF "+timeOff+"\r\n");
   String s = FPSTR(MainPage);
   webServer.send(200,"text/html",s);
}

void on_D1(){
  Serial.print("D1_ON \r\n");
  
  webServer.send(200,"text/html","Bat may bom");
}
void off_D1(){
  Serial.print("D1_OFF \r\n");
  webServer.send(200,"text/html","Tat may bom");
}
void on_D2(){
  Serial.print("D2_UPD 1\r\n");
  digitalWrite(D2,LOW);
  webServer.send(200,"text/html","Bat do am");
}
void off_D2(){
  Serial.print("D1_OFF 0\r\n");
  Serial.print("D2_UPD 0\r\n");
  digitalWrite(D2,HIGH);
  webServer.send(200,"text/html","Tat do am");
}
void on_D3(){
  Serial.print("D3_UPD 1\r\n");
  digitalWrite(D3,LOW);
  webServer.send(200,"text/html","Bat hen gio");
}
void off_D3(){
  Serial.print("D1_OFF 0\r\n");
  Serial.print("D3_UPD 0\r\n");  
  digitalWrite(D3,HIGH);
  webServer.send(200,"text/html","Tat hen gio");
}

void get_status(){
  String d1,d2,d3;
  if(digitalRead(D1)==1){
    d1 = "OFF";
  }else{
    d1 = "ON";
  }
  if(digitalRead(D2)==1){
    d2 = "OFF";
  }else{
    d2 = "ON";
  }
   if(digitalRead(D3)==1){
    d3 = "OFF";
  }else{
    d3 = "ON";
  };
  String s = "{\"D1\": \""+ d1 +"\",\"D2\": \""+ d2 +"\",\"D3\": \""+ d3 +"\",\"timeOn\": \""+ timeOn +"\",\"timeOff\": \""+ timeOff +"\"}";
  webServer.send(200,"application/json",s);
}
void setTimeRTC(){
  String times;
   times = webServer.arg("time");
   Serial.print("SET_T "+times+"\r\n");
}
