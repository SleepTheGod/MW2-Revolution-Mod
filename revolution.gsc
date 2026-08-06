// ============================================================
// MW2 REVOLUTION MOD - CUSTOM SCRIPT
// ============================================================

MOD_NAME = "SleepTheGod's MW2 Revolution";
MOD_VERSION = "3.0";
MOD_AUTHOR = "SleepTheGod";

function onModLoad() {
    println("========================================");
    println("  " + MOD_NAME + " v" + MOD_VERSION);
    println("  By " + MOD_AUTHOR);
    println("========================================");
    println("  Press F8 to open the menu");
    println("  Join the revolution!");
    println("========================================");
    loadSettings();
}

function startGunGame() {
    setDvar("scr_gun_game", "1");
    setDvar("scr_gun_game_weapons", "all");
    setDvar("scr_gun_game_timelimit", "10");
    setDvar("scr_gun_game_scorelimit", "30");
    setDvar("scr_gun_game_roundswitch", "1");
    println("Starting Gun Game mode!");
    sendMessage("^6Gun Game started! Get a kill with each weapon!");
}

function startInfection() {
    setDvar("scr_infection", "1");
    setDvar("scr_infection_rounds", "5");
    setDvar("scr_infection_timelimit", "5");
    setDvar("scr_infection_startplayers", "2");
    println("Starting Infection mode!");
    sendMessage("^4Infection mode started! The infection spreads!");
}

function startPropHunt() {
    setDvar("scr_prophunt", "1");
    setDvar("scr_prophunt_timelimit", "10");
    setDvar("scr_prophunt_rounds", "3");
    println("Starting Prop Hunt mode!");
    sendMessage("^5Prop Hunt started! Hide or seek!");
}

function startZombieMode() {
    setDvar("scr_zombies", "1");
    setDvar("scr_zombies_waves", "10");
    setDvar("scr_zombies_difficulty", "normal");
    println("Starting Zombie mode!");
    sendMessage("^7Zombie mode started! Survive the waves!");
}

function onPlayerConnect(player) {
    printf("Player %s joined the revolution!\n", player.name);
    sendMessage("%s, welcome to MW2 Revolution!", player.name);
    sendMessage("Check out the F8 menu for features!");
}

function onPlayerDisconnect(player) {
    printf("Player %s left the revolution.\n", player.name);
}

function onPlayerKilled(killer, victim, weapon) {
    if (killer == getLocalPlayer()) {
        printf("%s eliminated %s with %s\n", killer.name, victim.name, weapon);
        setKillstreak(killer);
    }
    if (weapon == "explosive") createExplosion(victim.position);
}

function onMatchStart() { println("Match started!"); sendMessage("^6Good luck, have fun!"); }
function onMatchEnd() { println("Match ended!"); sendMessage("^6Thanks for playing MW2 Revolution!"); }

function createFireworks(x, y, z) {
    for (i = 0; i < 50; i++) {
        createParticle("firework_spark", x + randomInt(100) - 50, y + randomInt(100) - 50, z + randomInt(50));
        wait(0.05);
    }
    println("Fireworks! 🎆");
}

function createMoneyRain() {
    for (i = 0; i < 100; i++) { createParticle("money_bill", randomInt(1920), randomInt(1080), 0); wait(0.02); }
    println("Money rain! 💰");
}

function createForceField(player) {
    let center = player.position;
    for (i = 0; i < 36; i++) {
        let angle = i * 10 * 3.14159 / 180;
        createParticle("force_field", center.x + cos(angle) * 100, center.y + sin(angle) * 100, center.z);
    }
    println("Force field activated!");
}

function createDisco() {
    for (i = 0; i < 10; i++) { setDvar("r_colorMap", randomColor()); wait(0.1); }
    setDvar("r_colorMap", "Unchanged");
}

function randomColor() {
    let colors = ["Red", "Green", "Blue", "Yellow", "Purple", "Cyan", "White"];
    return colors[randomInt(colors.size())];
}

function randomInt(max) { return int(random() * max); }
function randomFloat(max) { return random() * max; }
function lerp(a, b, t) { return a + (b - a) * t; }
function clamp(val, min, max) { return val < min ? min : (val > max ? max : val); }

function sendMessage(msg) { for (player in getPlayers()) player.iprintln(msg); }
function getPlayers() { return []; }
function getLocalPlayer() { return null; }
function getPlayerName(player) { return player ? player.name : "Unknown"; }
function createParticle(type, x, y, z) { }
function createExplosion(pos) { }
function setKillstreak(player) { }
function loadSettings() { println("Loading settings..."); }
function saveSettings() { println("Saving settings..."); }

exports = {
    onModLoad: onModLoad,
    startGunGame: startGunGame,
    startInfection: startInfection,
    startPropHunt: startPropHunt,
    startZombieMode: startZombieMode,
    onPlayerConnect: onPlayerConnect,
    onPlayerDisconnect: onPlayerDisconnect,
    onPlayerKilled: onPlayerKilled,
    onMatchStart: onMatchStart,
    onMatchEnd: onMatchEnd,
    createFireworks: createFireworks,
    createMoneyRain: createMoneyRain,
    createForceField: createForceField,
    createDisco: createDisco,
    randomInt: randomInt,
    randomFloat: randomFloat,
    lerp: lerp,
    clamp: clamp,
    sendMessage: sendMessage,
    getPlayers: getPlayers,
    getLocalPlayer: getLocalPlayer,
    getPlayerName: getPlayerName,
    createParticle: createParticle,
    createExplosion: createExplosion,
    loadSettings: loadSettings,
    saveSettings: saveSettings
};

onModLoad();
