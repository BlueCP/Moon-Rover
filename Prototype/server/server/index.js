const express = require('express')
const bodyParser = require('body-parser');
const cors = require('cors');
const path = require('path');

const app = express();
const port = 3000;

// Where we will keep books
let books = [];

app.use(cors());

// Configuring body parser middleware
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.post('/book', (req, res) => {
    // We will be coding here
});

app.get('/', function(req, res) {
    res.sendFile(path.join(__dirname, '/joystick.html'));
  });

app.listen(port, () => console.log(`Hello world app listening on port ${port}!`));



  

var httpapp = require('http').createServer(handler);
var io = require('socket.io')(httpapp)
var fs = require('fs');

var mySocket = 0;

httpapp.listen(4000); //Which port are we going to listen to?

function handler (req, res) {
  fs.readFile(__dirname + '/rover.html', //Load and display outputs to the index.html file
  function (err, data) {
    if (err) {
      res.writeHead(500);
      return res.end('Error loading index.html');
    }
    res.writeHead(200);
    res.end(data);
  });
}

io.sockets.on('connection', function (socket) {
  console.log('Webpage connected'); //Confirmation that the socket has connection to the webpage
  // mySocket = socket;
  //io.sockets.emit('field', 'test SUCCESS');
});
 
//UDP server on 41181
var dgram = require("dgram");
var server = dgram.createSocket("udp4");

server.on("message", function (msg, rinfo) {
  console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
  console.log("Broadcasting Message: " + msg); //Display the message coming from the terminal to the command line for debugging
  // if (mySocket != 0) {
  //    mySocket.emit('field', "" + msg);
  //    mySocket.broadcast.emit('field', "" + msg); //Display the message from the terminal to the webpage
  // }

  // io.sockets.emit('field', msg.toString());
  io.sockets.emit('field', JSON.stringify(processdata(msg)));

});

server.on("listening", function () {
  var address = server.address(); //IPAddress of the server
  console.log("UDP server listening to " + address.address + ":" + address.port);
});

server.bind(41181);


var result = {
  "radio61k": 151.53,
  "radio89k": 238.96,
  "infrared": 571.1}


function processdata(input){

  input = JSON.parse(input.toString('utf8'))

  // analysis data here
  var GaboriumConfidence = Math.abs(input.radio61k -  151) /151; 
  var LathwaiteConfidence = Math.abs(input.radio61k -  239) /239; 
  var AdamantineConfidence = Math.abs(input.radio89k -  151) /151; 
  var XirangConfidence = Math.abs(input.radio89k -  239) /239; 
  var ThiotimolineConfidence = Math.abs(input.infrared -  353) /353; 
  var NetheriteConfidence = Math.abs(input.infrared -  571) /571; 


  console.log(`GaboriumConfidence ${GaboriumConfidence*100} % \n`);
  console.log(`LathwaiteConfidence ${LathwaiteConfidence*100} % \n`);
  console.log(`AdamantineConfidence ${AdamantineConfidence*100} % \n`);
  console.log(`XirangConfidence ${XirangConfidence*100} % \n`);
  console.log(`ThiotimolineConfidence ${ThiotimolineConfidence*100} % \n`);
  console.log(`NetheriteConfidence ${NetheriteConfidence*100} % \n`);

  var maxConfidence = Math.max(GaboriumConfidence, LathwaiteConfidence, AdamantineConfidence, XirangConfidence, ThiotimolineConfidence, NetheriteConfidence)

  var material = "error"

  if(maxConfidence == GaboriumConfidence) material = "Gaborium";
  if(maxConfidence == LathwaiteConfidence) material = "Lathwaite";
  if(maxConfidence == AdamantineConfidence) material = "Adamantine";
  if(maxConfidence == XirangConfidence) material = "Xirang";
  if(maxConfidence == ThiotimolineConfidence) material = "Thiotimoline";
  if(maxConfidence == NetheriteConfidence) material = "Netherite";





  var res = JSON.parse(JSON.stringify(input));

  res["material"] = material
  res["GaboriumConfidence"] = GaboriumConfidence
  res["LathwaiteConfidence"] = LathwaiteConfidence
  res["AdamantineConfidence"] = AdamantineConfidence
  res["XirangConfidence"] = XirangConfidence
  res["ThiotimolineConfidence"] = ThiotimolineConfidence
  res["NetheriteConfidence"] = NetheriteConfidence


  //result.result1 = input.toString('utf8');

  return res;
}