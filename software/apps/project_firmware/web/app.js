/* jshint esversion: 6 */

/*
 * Server for ParkingBLE UI
 *
 * NOTE --- This is disorganized, we really should
 * have middleware, but there's not much time
 */

/*
 * Connection to the central device, fetching data
 */
const net = require('net');
const socket = new net.Socket();
const central_port = 42477;
const central_host = "localhost";

socket.on('error', (err) => console.log(err));

socket.on('data', (data) => {

    /*
     * Fetch the outputted data
     */
    const str = data.toString();
    const next_line = str.split(' ');


    /*
     * Parse the string for "RECV" prefix,
     * and check for [prefix, pid, st]
     */
    if (false
      	|| (next_line[0] != "RECV")
      	|| (next_line.length != 3)) return;


    /*
     * Set "parking_status" and "update_times"
     */
    const parking_ID = next_line[1],
	  status = Number(next_line[2]);

    parking_status[parking_ID] = status;
    update_times[parking_ID] = 0;

});

socket.connect(central_port, central_host, () => {
    console.log('connected to central device!');
});


/*
 * ParkingBLE state
 * - parking_status: [ pid : bool ]
 * - update_times: [ pid : mins since last update ]
 */
const parking_status = {
    "0" : 0,
    "1" : 0,
    "2" : 0,
    "3" : 0,
    "4" : 0,
    "5" : 0,
    "6" : 0,
    "7" : 0
};

const update_times = {
    "0" : 0,
    "1" : 0,
    "2" : 0,
    "3" : 0,
    "4" : 0,
    "5" : 0,
    "6" : 0,
    "7" : 0
};


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


/*
 * Increment "last updated" counters
 */
const increment_last_updated = () => {
    for (let i = 0; i < Object.keys(update_times).length; i++) update_times[i] += 1;
    setTimeout(increment_last_updated, 60000);
};

setTimeout(increment_last_updated, 60000);
