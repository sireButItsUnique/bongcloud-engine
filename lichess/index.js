const https = require("https");
const axios = require("axios");
const { spawn } = require("child_process");
require("dotenv").config();

async function listenGame(gameId) {
	let engine = spawn("../engine.exe", {
		stdio: ["pipe", "pipe", "pipe"],
	});

	engine.on("exit", (code, signal) => {
		console.log(`${gameId} exited with code ${code}`);
	});

	engine.stdout.on("data", (data) => {
		data = data.toString("utf-8");
		if (data.trim()) {
			console.log("engine says " + data);
		}
	});

	let req = https.request(
		{
			hostname: "lichess.org",
			path: "/api/bot/game/stream/" + gameId,
			method: "GET",
			headers: {
				Authorization: `Bearer ${process.env.API_TOKEN}`,
			},
		},
		(res) => {
			res.on("data", (data) => {
				let json = data.toString("utf-8");
				if (json.trim()) {
					json = JSON.parse(json);

					let state = false;
					if (json.type == "gameFull") {
						state = json.state;
					} else if (json.type == "gameState") {
						state = json;
					}

					if (state) {
						console.log(state);

						engine.stdin.write("lichessmove e2e4 lichesseval");
						engine.stdin.end();
					}
				}
			});
		}
	);

	req.end();
}

async function listenEvents() {
	let req = https.request(
		{
			hostname: "lichess.org",
			path: "/api/stream/event",
			method: "GET",
			headers: {
				Authorization: `Bearer ${process.env.API_TOKEN}`,
			},
		},
		(res) => {
			res.on("data", (data) => {
				let json = data.toString("utf-8");
				if (json.trim()) {
					json = JSON.parse(json);
					console.log(json.type);
					if (json.type == "challenge") {
						axios.post(
							`https://lichess.org/api/challenge/${json.challenge.id}/accept`,
							{},
							{
								headers: {
									Authorization: `Bearer ${process.env.API_TOKEN}`,
								},
							}
						);
					}

					if (json.type == "gameStart") {
						listenGame(json.game.gameId, json.game.color);
					}
				}
			});
		}
	);

	req.end();
}

// listenEvents();

// weird rules to use for engine:
// spawn engine ONCE
// do NOT call writeable.end() until game ends
let engine = spawn("../engine.exe");

engine.on("exit", (code, signal) => {
	console.log(`${gameId} exited with code ${code}`);
});

engine.stdout.on("data", (data) => {
	data = data.toString("utf-8");
	if (data.trim()) {
		console.log("engine says " + data);
	}
});

engine.stdin.write("lichessmove e2e4 print");
engine.stdin.write(" ");

engine.stdin.write("lichessmove g8h6 print lichesseval");
engine.stdin.write(" ");
