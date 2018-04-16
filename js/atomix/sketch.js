var player = {};
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
	height: 10,
	solution: {
		map: [4, 6, 5],
		w: 3,
		h: 1
	}
};

var spriteSheet = {
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
	frameRate(60);

	player.x = 0;
	player.y = 0;
	player.toX = 0;
	player.toY = 0;

	spriteSheet.image = loadImage('spritesheet.png');

	for (let i = 0; i < level.map.length; i++) {
		let spriteId = level.map[i];
		if (spriteId > 3) {
			let atom = {
				x: i % level.width,
				y: floor(i / level.width),
				spriteId: spriteId
			};
			atoms.push(atom);
		}
	}

	var modPlayer = new Modplayer();
	modPlayer.load("fairlight-cracktro.mod");
	//modPlayer.setamigamodel("500");
	//modPlayer.setautostart(true);

	background('black');
}

function draw() {
	scale(4);
	translate(0 * spriteSheet.gridSize, 0 * spriteSheet.gridSize);
	update();
	drawMap();
}

function update() {
	let speed = 0.2;
	player.x = player.x < player.toX ? min(player.toX, player.x + speed) : max(player.toX, player.x - speed);
	player.y = player.y < player.toY ? min(player.toY, player.y + speed) : max(player.toY, player.y - speed);

	if (player.selectedAtom != null) {
		let atom = atoms[player.selectedAtom];
		level.map[floor(atom.y) * level.width + floor(atom.x)] = 1;
		atom.x = player.x;
		atom.y = player.y;
		level.map[floor(atom.y) * level.width + floor(atom.x)] = atom.spriteId;
	}

	player.isMovingAtom = player.selectedAtom != null && (player.x != player.toX || player.y != player.toY);
	if (!player.isMovingAtom) {
		if (isLevelDone()) {
			console.log("yay!");
		}
	}
}

function isLevelDone() {
	let v1, x1, y1;
	for (let i = 0; i < level.solution.map.length; i++) {
		v1 = level.solution.map[i];
		if (v1 != 0) {
			x1 = i % level.solution.w;
			y1 = floor(i / level.solution.w);
			break;
		}
	}

	let v2, x2, y2;
	for (let i = 0; i < level.map.length; i++) {
		v2 = level.map[i];
		if (v1 == v2) {
			x2 = i % level.width;
			y2 = floor(i / level.width);
			break;
		}
	}

	x2 -= x1;
	y2 -= y1;
	for (let i = 0; i < level.solution.map.length; i++) {
		v1 = level.solution.map[i];
		if (v1 != 0) {
			x1 = i % level.solution.w;
			y1 = floor(i / level.solution.w);
			v2 = level.map[x1 + x2 + (y1 + y2) * level.width];
			if (v1 != v2) {
				return false;
			}
		}
	}

	return true;
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
		let x = atoms[i].x * gridSize;
		let y = atoms[i].y * gridSize;
		let spriteCoord = spriteSheet.coords[atoms[i].spriteId];
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

function stepsUntilBlock(direction) {
	let steps = 0;
	let playerIdx = floor(player.y) * level.width + floor(player.x);
	switch (direction) {
		case "LEFT":
			while (level.map[playerIdx - steps - 1] <= 1) steps++;
			break;
		case "RIGHT":
			while (level.map[playerIdx + steps + 1] <= 1) steps++;
			break;
		case "UP":
			while (level.map[playerIdx - (steps + 1) * level.width] <= 1) steps++;
			break;
		case "DOWN":
			while (level.map[playerIdx + (steps + 1) * level.width] <= 1) steps++;
			break;
	}
	return steps;
}

function keyPressed() {
	let steps = 1;
	if (!player.isMovingAtom) {
		switch (keyCode) {
			case LEFT_ARROW:
				if (player.selectedAtom != null) steps = stepsUntilBlock('LEFT');
				player.toX = max(0, player.toX - steps);
				break;
			case RIGHT_ARROW:
				if (player.selectedAtom != null) steps = stepsUntilBlock('RIGHT');
				player.toX = min(level.width - 1, player.toX + steps);
				break;
			case UP_ARROW:
				if (player.selectedAtom != null) steps = stepsUntilBlock('UP');
				player.toY = max(0, player.toY - steps);
				break;
			case DOWN_ARROW:
				if (player.selectedAtom != null) steps = stepsUntilBlock('DOWN');
				player.toY = min(level.height - 1, player.toY + steps);
				break;
			case 32: // SPACEBAR
				if (player.selectedAtom == null) {
					for (i in atoms) {
						let atom = atoms[i];
						if (player.x == atom.x && player.y == atom.y) {
							player.selectedAtom = i;
							break;
						}
					}
				} else {
					player.selectedAtom = null;
				}
				break;
		}
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
