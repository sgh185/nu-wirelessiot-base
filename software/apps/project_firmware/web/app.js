/*
 * Server for ParkingBLE UI
 *
 * NOTE --- This is disorganized, we really should
 * have middleware, but there's not much time for
 * that shit
 */

/*
 * Connection to the central device, fetching data
 */
const net = require('net')
const socket = new net.Socket()
const central_port = 42477;
const central_host = "localhost";

socket.on('error', (err) => console.log(err))

socket.on('data', (data) => console.log(data.toString()))

socket.connect(central_port, central_host, () => {
    console.log('connected to central device!');
});


/*
 * ParkingBLE state
 * - parking_status: [ pid : bool ]
 * - update_times: [ pid : mins since last update ]
 */
const parking_status = {};
const update_times = {};


/*
 * Express setup
 */
const express = require('express');
const ejs = require('ejs');

const app = express();
app.set('view engine', 'ejs');
app.use(express.static("public"));


/*
 * Routes setup
 */
app.get("/*", (req, res) => {
  res.render(
    "index", {
      ps: parking_status,
      ut: update_times
    }
  );
});


/*
 * Port setup
 */
let port = process.env.PORT;
if (port == null || port == "") {
  port = 3000;
}

app.listen(port, () => {
  console.log("Server started.");
});
