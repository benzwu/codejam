var wave;
var env;
var time;
var x, y;
var prime = 2;

function setup() {
	createCanvas(windowWidth, windowHeight);
	x = windowWidth / 2;
	y = windowHeight / 2;

	env = new p5.Env();
	env.setADSR(0.001, 0.05, 0, 0);
	env.setRange(1, 0);

	wave = new p5.Oscillator();
	wave.setType('sine');
	wave.start();
	wave.freq(440);
	wave.amp(env);

	noStroke();
	colorMode(HSB);
	background(240, 255, 255);
}

function draw() {
    var size = 50;
    var inc = 10;
    var speed = 500;

    var now = Math.floor(millis() / speed);
	if (time != now) {
		time = now;
		env.play();
		switch (prime % 4) {
			case 0:
				x -= size;
	            wave.freq(200);
				break;
			case 1:
				x += size;
	            wave.freq(300);
				break;
			case 2:
				y -= size;
	            wave.freq(400);
				break;
			case 3:
				y += size;
	            wave.freq(500);
				break;
		}
	    colorMode(RGB);
        var h = max(0, hue(get(x, y)) - inc);
	    colorMode(HSB);
		fill(h, 255, 255);
		rect(x - size / 2, y - size / 2, size, size);
        console.log(prime);
        prime = nextPrime(prime);
	}
}

function nextPrime(n) {
    return 2;
    do {
        for (var i = 2; i < n; i++) {
            if (n % i == 0) {
                n++;
                break;
            }
        }
    } while (true);
}
