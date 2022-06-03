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
  console.log("Broadcasting Message: " + msg); //Display the message coming from the terminal to the command line for debugging
  // if (mySocket != 0) {
  //    mySocket.emit('field', "" + msg);
  //    mySocket.broadcast.emit('field', "" + msg); //Display the message from the terminal to the webpage
  // }
  processdata(msg)
  io.sockets.emit('field', JSON.stringify(result));

});

server.on("listening", function () {
  var address = server.address(); //IPAddress of the server
  console.log("UDP server listening to " + address.address + ":" + address.port);
});

server.bind(41181);


var result = {
  result1: "r1",
  result12: "r2",
}


function processdata(input){

  // analysis data here
  result.result1 = input.toString('utf8');

  return result;
}