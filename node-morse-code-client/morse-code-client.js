var net = require('net');

exports.send = function(message) {
  var client = net.connect({host: '192.168.0.73', port: 73}, function() {
    client.write(message);
    client.end();
  });
};
