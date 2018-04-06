var player = {
	x: 0,
	y: 0,
	toX: null,
	toY: null,
	speed: 0.2,
	selectedAtom: null
};

var atoms = [];

var level = {
	map: [
		2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0,
		2, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0,
		2, 1, 1, 5, 3, 2, 2, 2, 2, 2, 0,
		2, 1, 1, 3, 1, 1, 1, 1, 1, 2, 0,
		2, 1, 3, 1, 1, 1, 1, 1, 1, 3, 2,
		2, 1, 3, 1, 1, 3, 1, 3, 3, 1, 2,
		2, 1, 1, 1, 1, 3, 1, 3, 6, 1, 2,
		2, 3, 3, 4, 3, 1, 1, 3, 1, 1, 2,
		0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2,
		0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2],
	width: 11,
	height: 10
};

var spriteSheet = {
	image: null,
	coords: {
		1: [0, 48, 16, 16],
		2: [0, 0, 16, 16],
		3: [16, 0, 16, 16],
		4: [0, 16, 16, 16],
		5: [16, 16, 16, 16],
		6: [0, 32, 16, 16]},
	gridSize: 16
};

function setup() {
	createCanvas(windowWidth, windowHeight);

	noSmooth();
	frameRate(30);

	spriteSheet.image = loadImage('spritesheet.png');

	for (let i = 0; i < level.map.length; i++) {
		let spriteId = level.map[i];
		if (spriteId > 3) {
			atoms[i] = spriteId;
		}
	}

	var modPlayer = new Modplayer();
	modPlayer.load("fairlight-cracktro.mod");
	modPlayer.setautostart(true);
	modPlayer.setamigamodel("500");

	background('black');
}

function draw() {
	scale(4);
	translate(1 * spriteSheet.gridSize, 1 * spriteSheet.gridSize);
	update();
	drawMap();
}

function update() {
	if (player.toX != null) {
		player.x = player.x < player.toX ? min(player.toX, player.x + player.speed) : max(player.toX, player.x - player.speed);
	}
	if (player.toY != null) {
		player.y = player.y < player.toY ? min(player.toY, player.y + player.speed) : max(player.toY, player.y - player.speed);
	}
	if (player.selectedAtom != null) {
		; // TODO
	}
}

function drawMap() {
	let gridSize = spriteSheet.gridSize;

	// background
	noStroke();
	fill(65, 48, 0);
	rect(0, 0, level.width * gridSize, level.height * gridSize);

	// walls
	for (let i = 0; i < level.map.length; i++) {
		let x = (i % level.width) * gridSize;
		let y = floor(i / level.width) * gridSize;
		let spriteId = level.map[i];
		if (spriteId == 0) {
			fill('black');
			rect(x, y, gridSize, gridSize);
		} else if (spriteId == 2 || spriteId == 3) {
			let spriteCoord = spriteSheet.coords[spriteId];
			image(spriteSheet.image, x, y, spriteCoord[2], spriteCoord[3], spriteCoord[0], spriteCoord[1], spriteCoord[2], spriteCoord[3]);
		}
	}

	// atoms
	for (i in atoms) {
		let spriteId = atoms[i];
		let x = (i % level.width) * gridSize;
		let y = floor(i / level.width) * gridSize;
		let spriteCoord = spriteSheet.coords[spriteId];
		image(spriteSheet.image, x, y, spriteCoord[2], spriteCoord[3], spriteCoord[0], spriteCoord[1], spriteCoord[2], spriteCoord[3]);
	}

	// player
	let x = player.x * gridSize;
	let y = player.y * gridSize;
	let spriteCoord = spriteSheet.coords[1];
	noFill();
	if (player.selectedAtom == null) {
		strokeWeight(2);
		stroke(220, 0, 0);
		rect(x + 1, y + 1, gridSize - 2, gridSize - 2, 1);
	} else {
		strokeWeight(1.2);
		stroke('white');
		rotatingDashedEllipse(x + gridSize / 2 - 1, y + gridSize / 2 - 1, gridSize * 0.3);
	}
}

function keyPressed() {
	switch (keyCode) {
		case LEFT_ARROW:
			player.toX = max(0, player.toX - 1);
			break;
		case RIGHT_ARROW:
			player.toX = min(level.width - 1, player.toX + 1);
			break;
		case UP_ARROW:
			player.toY = max(0, player.toY - 1);
			break;
		case DOWN_ARROW:
			player.toY = min(level.height - 1, player.toY + 1);
			break;
		case 32: // SPACEBAR
			selectedAtom();
			break;
	}
}

function selectedAtom() {
	if (player.selectedAtom == null) {
		let idx = player.y * level.width + player.x;
		if (atoms[idx] != null) {
			player.selectedAtom = idx;
		}
	} else {
		player.selectedAtom = null;
	}
}

function rotatingDashedEllipse(x, y, r) {
	push();
	translate(x, y);
	rotate(millis() * 0.005);

	let x1 = r;
	let y1 = 0;
	let detail = 16;
	for (let i = 0; i < detail; i++) {
		let a = (i + 1) * TWO_PI / detail;
		let x2 = r * cos(a);
		let y2 = r * sin(a);
		if (i % 2) line(x1, y1, x2, y2);
		x1 = x2;
		y1 = y2;
	}

	pop();
}
