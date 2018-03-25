var wave, env, delay;
var time;
var x, y;
var prime = 2;

var boxSize = 30;
var inc = 20;
var bpm = 120;
var pitch = {
	A: 220.00,
	B: 246.94,
	C: 261.63,
	D: 293.66,
	E: 329.63,
	F: 349.23,
	G: 392.00,
	A2: 440,
	A9: 2732,
	B9: 3625,
	C9: 3290,
	D9: 2343
};

function setup() {
	createCanvas(windowWidth, windowHeight);
	x = windowWidth / 2;
	y = windowHeight / 2;

	wave = new p5.Oscillator();
	wave.setType('sine');
	wave.start();

	delay = new p5.Delay();
	delay.process(wave);
	delay.delayTime(0.2);
	delay.feedback(0.2);

	env = new p5.Env();
	env.setADSR(0.01, 0.03, 0, 0);

	colorMode(HSB);
	background(240, 255, 255);
}

function draw() {
	var now = Math.floor(bpm * millis() / 60000);
	if (time != now) {
		time = now;

		bpm = min(bpm + 0.1, 10000);

		colorMode(RGB);
		var h = max(0, hue(get(x, y)) - inc);
		colorMode(HSB);
		stroke(h, 255, 255);
		fill(h, 255, 255);
		rect(x - boxSize / 2, y - boxSize / 2, boxSize, boxSize);
		switch (prime % 5) {
			case 1:
				x -= boxSize;
				wave.freq(pitch[h > 150 ? 'A' : h > 10 ? 'B' : 'A9']);
				break;
			case 2:
				x += boxSize;
				wave.freq(pitch[h > 150 ? 'C' : h > 10 ? 'D' : 'B9']);
				break;
			case 3:
				y -= boxSize;
				wave.freq(pitch[h > 150 ? 'E' : h > 10 ? 'F' : 'C9']);
				break;
			case 4:
				y += boxSize;
				wave.freq(pitch[h > 150 ? 'G' : h > 10 ? 'A2' : 'D9']);
				break;
		}
		env.play(wave);

		prime = nextPrime(prime);
	}
}

function nextPrime(number) {
	number++;
	do {
		var start = 2;
		var found = true;
		while (start <= sqrt(number)) {
			if (number % start++ < 1) {
				number++;
				found = false;
				break;
			}
		}
		if (found) return number;
	} while (true);
}
