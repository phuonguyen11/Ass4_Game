const { randomInt } = require("crypto");
const express = require("express"); // use express
const app = express();
const server = require("http").Server(app);
const io = require("socket.io")(server, { allowEIO3: true, });

PORT = 8080;
IP = '172.20.79.176';

var turn;
var pause = 0;

// Store the list of connected players
let players = [];

// Store the list of game rooms
let rooms = {};
server.listen(PORT, function () {
    console.log('Server running at port ' + PORT);
});

io.on('connection', function (socket) {
    console.log('A user connected');

    // Assign a unique ID to the player
    const playerId = socket.id;

    socket.emit("message", "hello");

    // Player joins a room
    socket.on('join_room', (roomId) => {
        console.log('Client', socket.id, 'joined room', roomId);
        socket.join(roomId);

        // Check if the room exists
        if (!rooms[roomId]) {
            // Create the room if it doesn't exist
            rooms[roomId] = { playerCount: 0 };
        }

        const room = rooms[roomId];

        // Check if the room is full
        if (room.playerCount >= 2) {
            socket.emit('join_room', -1); // Send -1 as player index to indicate room is full
            return;
        }

        if (room.playerCount == 0) {
            turn = randomInt(0, 1);
        }

        const playerTurn = turn;
        const playerType = turn;
        turn = (turn + 1) % 2;

        console.log(playerTurn, playerType);

        // Increment player count and assign player index
        const playerIndex = room.playerCount;
        room.playerCount++;

        // Store the player in the players list
        players.push({ socket, roomId, playerIndex, playerTurn, playerType });

        // Notify the player about successful room join
        socket.emit('join_room', playerIndex);

        // Notify other players in the room about the new player
        socket.to(roomId).emit('player_join', playerIndex);

        // Join the room's socket.io room
        socket.join(roomId);

        // If two players have joined, start the game
        if (room.playerCount === 2) {
            io.to(roomId).emit('start_game');
            io.to(players[0].socket.id).emit('swap_turn', players[0].playerTurn);
            io.to(players[1].socket.id).emit('swap_turn', players[1].playerTurn);
            io.to(players[0].socket.id).emit('get_type', players[0].playerType);
            io.to(players[1].socket.id).emit('get_type', players[1].playerType);
        }
    });

    // Player makes a move
    socket.on('player_move', (roomId, posX, posY) => {
        console.log("player moving at " + roomId +" " + "(" + posX + ";" + posY + ")");
        // Find the player's room
        const room = rooms[roomId];

        // Find the opponent player
        const opponent = players.find(
            (player) => player.roomId === roomId && player.socket !== socket
        );

        if (!opponent) {
            return;
        };

        // Broadcast the move to both players in the room
        io.to(roomId).emit('player_move', posX + posY);
        io.to(players[0].socket.id).emit('swap_turn', players[1].playerTurn);
        io.to(players[1].socket.id).emit('swap_turn', players[0].playerTurn);
        const temp = players[0].playerTurn;
        players[0].playerTurn = players[1].playerTurn;
        players[1].playerTurn = temp;

        if (pause == 1) {
            io.to(opponent.socket.id).emit('stop_pause', pause);
            pause = 0;
        };
    });

    socket.on("load_state", (msg) => {
        console.log("Load current state " + msg);
    });

    // test
    socket.on("test", (data) => {
        console.log("test " + data);
    });

    socket.on("winner", (roomId, data) => {
        console.log("The winner is player: " + data);

        // Find the player's room
        const room = rooms[roomId];

        io.to(roomId).emit('winner', data);
    });

    socket.on("pause", (roomId, data) => {
        console.log("At room " + roomId + ": Player " + data + " wanna pause the game!");

        // Find the player's room
        const room = rooms[roomId];

        // Find the opponent player
        const opponent = players.find(
            (player) => player.roomId === roomId && player.socket !== socket
        );

        if (!opponent) {
            return;
        };

        io.to(roomId).emit('pause', data);
        pause = 1;
    });

    // Player disconnects
    socket.on('disconnect', () => {
        console.log('A user disconnected');

        // Find the player and remove them from the players list
        const playerIndex = players.findIndex((player) => player.socket === socket);

        if (playerIndex === -1) {
            return;
        }

        const player = players[playerIndex];
        players.splice(playerIndex, 1);

        // Find the player's room
        const room = rooms[player.roomId];

        if (!room) {
            return;
        }

        // Decrement the player count
        room.playerCount--;

        // Notify the remaining player about the disconnection
        socket.to(player.roomId).emit('opponent_disconnected');

        // Remove the room if there are no players left
        if (room.playerCount === 0) {
            delete rooms[player.roomId];
        };
        pause = 0;
    });

})
