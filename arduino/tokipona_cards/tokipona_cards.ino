/**
* Toki Pona Cards LCDKeypad version
*
* Toki Pona official site: https://tokipona.org/
* Code: https://github.com/maks-ushakov/arduino-experiments/tree/master/arduino
* Simulation: https://www.tinkercad.com/things/hrG1mFflU87-toki-pona-cards
*
* Buttons:
* Select: new word
* Any dir: show translation
*/
#include <LiquidCrystal.h>

#define LCD_RS 8
#define LCD_EN 9
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define KEYPAD A0

#define LCD_SIZE 16
#define BUFFER_SIZE (LCD_SIZE + 2)
enum {
  KEY_SEL,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_UP,
  KEY_DOWN,
  KEY_UNPRESS
};

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
unsigned long previousCheckTime = 0;
unsigned long previousRenderTime = 0;
uint8_t codeLength = 4;
uint8_t present = 0;
uint8_t rightGuess = 0;
int dictSize = 115;
bool flipStatus = false;
char currentWord[BUFFER_SIZE];
char currentTranslation[BUFFER_SIZE];

long renderTime = 100;
long checkTime = 200;
int sensorValue = KEY_UNPRESS;
int previousValue = KEY_UNPRESS;
int outValue = KEY_UNPRESS;
bool is_changed = false;

void setup()
{
  lcd.begin(16,2);
  lcd.print("toki pona cards");
  lcd.setCursor(0, 1);
  lcd.print("Press any key");
  pinMode(KEYPAD, INPUT);
  while(sensorValue == KEY_UNPRESS) {
    sensorValue = parseKey(analogRead(KEYPAD));
  }
  randomSeed(analogRead(A5));
  nextWord();
  is_changed = true;
}

void loop()
{
  unsigned long currentTime = millis();
  if (currentTime - previousCheckTime > checkTime && !is_changed) {
    sensorValue = parseKey(analogRead(KEYPAD));
    if(sensorValue != outValue) {
      previousValue = outValue;
      outValue = sensorValue;
      is_changed = true;
      handleKey(outValue);
    }
  }

  if (currentTime - previousRenderTime > renderTime && is_changed) {
    lcd.clear();
    lcd.setCursor(0, 0);
	lcd.print(currentWord);
    if(flipStatus) {
      lcd.setCursor(0, 1);
      lcd.print(currentTranslation);
    }
    is_changed = false;
  }
}

int parseKey (int keyValue) {
  if (keyValue > 1020) {
    return KEY_UNPRESS;
  }
  if (keyValue > 720) {
    return KEY_SEL;
  }
  if (keyValue > 500) {
    return KEY_LEFT;
  }
  if (keyValue > 320) {
    return KEY_DOWN;
  }
  if (keyValue > 140) {
    return KEY_UP;
  }
  return KEY_RIGHT;
}

String printKey(int key) {
  switch(key) {
    case KEY_SEL:   return "SELECT"; break;
    case KEY_LEFT:  return "LEFT"; break;
    case KEY_UP:    return "UP"; break;
    case KEY_DOWN:  return "DOWN"; break;
    case KEY_RIGHT:  return "RIGHT"; break;
    default: return "unpressed";
  }
}

void nextWord() {
	int index = random(0, dictSize);
  // strcpy_P(currentWord[0], (char *)pgm_read_ptr(&(tokiDictionary[index][0])));
  // strcpy_P(currentWord[1], (char *)pgm_read_ptr(&(tokiDictionary[index][1])));
  flipStatus = false;
  getDictionaryWord(index);
  getDictionaryTranslation(index);
}

void flip() {
  flipStatus = !flipStatus;
}

void pass() {};
void handleKey(uint8_t key) {
  switch(key) {
    case KEY_SEL: nextWord(); break;
    case KEY_LEFT:  
    case KEY_RIGHT:  
    case KEY_UP:    
    case KEY_DOWN:  flip(); break;

    default: pass();
  }
}

const char * const tokiDictionary[][2] = {
  {"akesi", "reptile"},
  {"ala", "no, not, zero"},
  {"alasa", "to hunt"},
  {"ale (ali)", "all; 100"},
  {"anpa", "humble, lowly"},
  {"ante", "different"},
  {"awen", "kept,to continue"},
  {"esun", "market, shop"}
};

const char dw_000[] PROGMEM = "akesi";
const char dt_000[] PROGMEM = "reptile";
const char dw_001[] PROGMEM = "ala";
const char dt_001[] PROGMEM = "no, not, zero";
const char dw_002[] PROGMEM = "alasa";
const char dt_002[] PROGMEM = "to hunt";
const char dw_003[] PROGMEM = "ale (ali)";
const char dt_003[] PROGMEM = "all; 100";
const char dw_004[] PROGMEM = "anpa";
const char dt_004[] PROGMEM = "humble, lowly";

const char dw_005[] PROGMEM = "ante";
const char dt_005[] PROGMEM = "different";
const char dw_006[] PROGMEM = "awen";
const char dt_006[] PROGMEM = "kept,to continue";
const char dw_007[] PROGMEM = "esun";
const char dt_007[] PROGMEM = "market, shop";
const char dw_008[] PROGMEM = "ijo";
const char dt_008[] PROGMEM = "thing, object";
const char dw_009[] PROGMEM = "ike";
const char dt_009[] PROGMEM = "bad, negative";

const char dw_010[] PROGMEM = "ilo";
const char dt_010[] PROGMEM = "tool, machine";
const char dw_011[] PROGMEM = "insa";
const char dt_011[] PROGMEM = "centre, inside";
const char dw_012[] PROGMEM = "jaki";
const char dt_012[] PROGMEM = "unclean, toxic";
const char dw_013[] PROGMEM = "jan";
const char dt_013[] PROGMEM = "person,somebody";
const char dw_014[] PROGMEM = "jelo";
const char dt_014[] PROGMEM = "yellow";

const char dw_015[] PROGMEM = "jo";
const char dt_015[] PROGMEM = "to have, contain";
const char dw_016[] PROGMEM = "kala";
const char dt_016[] PROGMEM = "sea creature";
const char dw_017[] PROGMEM = "kalama";
const char dt_017[] PROGMEM = "sound";
const char dw_018[] PROGMEM = "kama";
const char dt_018[] PROGMEM = "arriving, future";
const char dw_019[] PROGMEM =   "kasi";
const char dt_019[] PROGMEM = "plant, leaf";

const char dw_020[] PROGMEM = "ken";
const char dt_020[] PROGMEM = "can, possible";
const char dw_021[] PROGMEM = "kepeken";
const char dt_021[] PROGMEM = "to use, with";
const char dw_022[] PROGMEM = "kili";
const char dt_022[] PROGMEM = "fruit, vegetable";
const char dw_023[] PROGMEM = "kin";
const char dt_023[] PROGMEM = "also, too";
const char dw_024[] PROGMEM = "kipisi";
const char dt_024[] PROGMEM = "to cut, part";

const char dw_025[] PROGMEM = "kiwen";
const char dt_025[] PROGMEM = "metal, stone";
const char dw_026[] PROGMEM = "ko";
const char dt_026[] PROGMEM = "clay, paste";
const char dw_027[] PROGMEM = "kon";
const char dt_027[] PROGMEM = "air, spirit";
const char dw_028[] PROGMEM = "kule";
const char dt_028[] PROGMEM = "color";
const char dw_029[] PROGMEM = "kulupu";
const char dt_029[] PROGMEM = "group, company";

const char dw_030[] PROGMEM = "kute";
const char dt_030[] PROGMEM = "ear; to hear";
const char dw_031[] PROGMEM = "lape";
const char dt_031[] PROGMEM = "sleeping,resting";
const char dw_032[] PROGMEM = "laso";
const char dt_032[] PROGMEM = "blue, green";
const char dw_033[] PROGMEM = "lawa";
const char dt_033[] PROGMEM = "head, to lead";
const char dw_034[] PROGMEM = "leko";
const char dt_034[] PROGMEM = "square, block";

const char dw_035[] PROGMEM = "len";
const char dt_035[] PROGMEM = "cloth,cover";
const char dw_036[] PROGMEM = "lete";
const char dt_036[] PROGMEM = "cool, cold; raw";
const char dw_037[] PROGMEM = "lili";
const char dt_037[] PROGMEM = "small, few";
const char dw_038[] PROGMEM = "linja";
const char dt_038[] PROGMEM = "string, hair";
const char dw_039[] PROGMEM = "lipu";
const char dt_039[] PROGMEM = "paper, document";

const char dw_040[] PROGMEM = "loje";
const char dt_040[] PROGMEM = "red, reddish";
const char dw_041[] PROGMEM = "lon";
const char dt_041[] PROGMEM = "located at, true";
const char dw_042[] PROGMEM = "luka";
const char dt_042[] PROGMEM = "arm, hand; five";
const char dw_043[] PROGMEM = "lukin";
const char dt_043[] PROGMEM = "to see, examine";
const char dw_044[] PROGMEM = "lupa";
const char dt_044[] PROGMEM = "door, hole";

const char dw_045[] PROGMEM = "ma";
const char dt_045[] PROGMEM = "earth, land";
const char dw_046[] PROGMEM = "mama";
const char dt_046[] PROGMEM = "parent; creator";
const char dw_047[] PROGMEM = "mani";
const char dt_047[] PROGMEM = "money, cash";
const char dw_048[] PROGMEM = "meli";
const char dt_048[] PROGMEM = "Female";
const char dw_049[] PROGMEM = "mije";
const char dt_049[] PROGMEM = "male";

const char dw_050[] PROGMEM = "mi";
const char dt_050[] PROGMEM = "I, me, my";
const char dw_051[] PROGMEM = "moku";
const char dt_051[] PROGMEM = "to eat, food";
const char dw_052[] PROGMEM = "moli";
const char dt_052[] PROGMEM = "dead, dying";
const char dw_053[] PROGMEM = "monsi";
const char dt_053[] PROGMEM = "back, behind";
const char dw_054[] PROGMEM = "monsuta";
const char dt_054[] PROGMEM = "monster";

const char dw_055[] PROGMEM = "mun";
const char dt_055[] PROGMEM = "night sky object";
const char dw_056[] PROGMEM = "musi";
const char dt_056[] PROGMEM = "fun";
const char dw_057[] PROGMEM = "mute";
const char dt_057[] PROGMEM = "many, a lot";
const char dw_058[] PROGMEM = "namako";
const char dt_058[] PROGMEM = "spice, additional";
const char dw_059[] PROGMEM = "nanpa";
const char dt_059[] PROGMEM = "number";

const char dw_060[] PROGMEM = "nasa";
const char dt_060[] PROGMEM = "strange";
const char dw_061[] PROGMEM = "nasin";
const char dt_061[] PROGMEM = "way, method";
const char dw_062[] PROGMEM = "nena";
const char dt_062[] PROGMEM = "bump, hill, nose";
const char dw_063[] PROGMEM = "ni";
const char dt_063[] PROGMEM = "this, that";
const char dw_064[] PROGMEM = "nimi";
const char dt_064[] PROGMEM = "word, name";

const char dw_065[] PROGMEM = "noka";
const char dt_065[] PROGMEM = "foot, leg";
const char dw_066[] PROGMEM = "olin";
const char dt_066[] PROGMEM = "to love, respect";
const char dw_067[] PROGMEM = "open";
const char dt_067[] PROGMEM = "to begin, to start";
const char dw_068[] PROGMEM = "pakala";
const char dt_068[] PROGMEM = "broken, damaged";
const char dw_069[] PROGMEM = "pali";
const char dt_069[] PROGMEM = "to do, to work on";

const char dw_070[] PROGMEM = "palisa";
const char dt_070[] PROGMEM = "rod, stick";
const char dw_071[] PROGMEM = "pan";
const char dt_071[] PROGMEM = "bread, rice";
const char dw_072[] PROGMEM = "pana";
const char dt_072[] PROGMEM = "to give, to send";
const char dw_073[] PROGMEM = "pilin";
const char dt_073[] PROGMEM = "heart, feeling";
const char dw_074[] PROGMEM = "pimeja";
const char dt_074[] PROGMEM = "black, dark";

const char dw_075[] PROGMEM = "pini";
const char dt_075[] PROGMEM = "past, end";
const char dw_076[] PROGMEM = "pipi";
const char dt_076[] PROGMEM = "bug, insect";
const char dw_077[] PROGMEM = "poka";
const char dt_077[] PROGMEM = "hip, side";
const char dw_078[] PROGMEM = "poki";
const char dt_078[] PROGMEM = "box, bowl, cup";
const char dw_079[] PROGMEM = "pona";
const char dt_079[] PROGMEM = "good, simple";


const char dw_080[] PROGMEM = "sama";
const char dt_080[] PROGMEM = "similar, same";
const char dw_081[] PROGMEM = "seli";
const char dt_081[] PROGMEM = "Fire";
const char dw_082[] PROGMEM = "selo";
const char dt_082[] PROGMEM = "skin, peel";
const char dw_083[] PROGMEM = "seme";
const char dt_083[] PROGMEM = "what? which?";
const char dw_084[] PROGMEM = "sewi";
const char dt_084[] PROGMEM = "area above";

const char dw_085[] PROGMEM = "sijelo";
const char dt_085[] PROGMEM = "body, torso";
const char dw_086[] PROGMEM = "sike";
const char dt_086[] PROGMEM = "cycle";
const char dw_087[] PROGMEM = "sin";
const char dt_087[] PROGMEM = "new, another";
const char dw_088[] PROGMEM = "sinpin";
const char dt_088[] PROGMEM = "face,front,wall";
const char dw_089[] PROGMEM = "sitelen";
const char dt_089[] PROGMEM = "image, picture";

const char dw_090[] PROGMEM = "soko";
const char dt_090[] PROGMEM = "mushroom";
const char dw_091[] PROGMEM = "sona";
const char dt_091[] PROGMEM = "knowledge, info";
const char dw_092[] PROGMEM = "soweli";
const char dt_092[] PROGMEM = "land animal";
const char dw_093[] PROGMEM = "suli";
const char dt_093[] PROGMEM = "big,heavy,large";
const char dw_094[] PROGMEM = "suno";
const char dt_094[] PROGMEM = "sun, bright";

const char dw_095[] PROGMEM = "supa";
const char dt_095[] PROGMEM = "surface";
const char dw_096[] PROGMEM = "suwi";
const char dt_096[] PROGMEM = "sweet; cute";
const char dw_097[] PROGMEM = "tan";
const char dt_097[] PROGMEM = "from, because";
const char dw_098[] PROGMEM = "taso";
const char dt_098[] PROGMEM = "but, however";
const char dw_099[] PROGMEM = "tawa";
const char dt_099[] PROGMEM = "going to; for";


const char dw_100[] PROGMEM = "telo";
const char dt_100[] PROGMEM = "liquid, water";
const char dw_101[] PROGMEM = "tenpo";
const char dt_101[] PROGMEM = "time, period";
const char dw_102[] PROGMEM = "toki";
const char dt_102[] PROGMEM = "to say; language";
const char dw_103[] PROGMEM = "tomo";
const char dt_103[] PROGMEM = "building, home";
const char dw_104[] PROGMEM = "tonsi";
const char dt_104[] PROGMEM = "non-binary, trans";

const char dw_105[] PROGMEM = "tu";
const char dt_105[] PROGMEM = "Two";
const char dw_106[] PROGMEM = "unpa";
const char dt_106[] PROGMEM = "sex; to have sex.";
const char dw_107[] PROGMEM = "uta";
const char dt_107[] PROGMEM = "mouth, lips";
const char dw_108[] PROGMEM = "utala";
const char dt_108[] PROGMEM = "challenge, battle";
const char dw_109[] PROGMEM = "walo";
const char dt_109[] PROGMEM = "white, pale";


const char dw_110[] PROGMEM = "wan";
const char dt_110[] PROGMEM = "one; unique";
const char dw_111[] PROGMEM = "waso";
const char dt_111[] PROGMEM = "flying creature";
const char dw_112[] PROGMEM = "wawa";
const char dt_112[] PROGMEM = "strong, powerful";
const char dw_113[] PROGMEM = "weka";
const char dt_113[] PROGMEM = "absent, away";
const char dw_114[] PROGMEM = "wile";
const char dt_114[] PROGMEM = "must, need, want";

const char *const dictionaryWords[] PROGMEM = {
  dw_000, dw_001, dw_002, dw_003, dw_004, dw_005, dw_006, dw_007, dw_008, dw_009,
  dw_010, dw_011, dw_012, dw_013, dw_014, dw_015, dw_016, dw_017, dw_018, dw_019,
  dw_020, dw_021, dw_022, dw_023, dw_024, dw_025, dw_026, dw_027, dw_028, dw_029,
  dw_030, dw_031, dw_032, dw_033, dw_034, dw_035, dw_036, dw_037, dw_038, dw_039,
  dw_040, dw_041, dw_042, dw_043, dw_044, dw_045, dw_046, dw_047, dw_048, dw_049,
  dw_050, dw_051, dw_052, dw_053, dw_054, dw_055, dw_056, dw_057, dw_058, dw_059,
  dw_060, dw_061, dw_062, dw_063, dw_064, dw_065, dw_066, dw_067, dw_068, dw_069,
  dw_070, dw_071, dw_072, dw_073, dw_074, dw_075, dw_076, dw_077, dw_078, dw_079,
  dw_080, dw_081, dw_082, dw_083, dw_084, dw_085, dw_086, dw_087, dw_088, dw_089,
  dw_090, dw_091, dw_092, dw_093, dw_094, dw_095, dw_096, dw_097, dw_098, dw_099,
  dw_100, dw_101, dw_102, dw_103, dw_104, dw_105, dw_106, dw_107, dw_108, dw_109,
  dw_110, dw_111, dw_112, dw_113, dw_114,
};

const char *const dictionaryTranslations[] PROGMEM = {
  dt_000, dt_001, dt_002, dt_003, dt_004, dt_005, dt_006, dt_007, dt_008, dt_009,
  dt_010, dt_011, dt_012, dt_013, dt_014, dt_015, dt_016, dt_017, dt_018, dt_019,
  dt_020, dt_021, dt_022, dt_023, dt_024, dt_025, dt_026, dt_027, dt_028, dt_029,
  dt_030, dt_031, dt_032, dt_033, dt_034, dt_035, dt_036, dt_037, dt_038, dt_039,
  dt_040, dt_041, dt_042, dt_043, dt_044, dt_045, dt_046, dt_047, dt_048, dt_049,
  dt_050, dt_051, dt_052, dt_053, dt_054, dt_055, dt_056, dt_057, dt_058, dt_059,
  dt_060, dt_061, dt_062, dt_063, dt_064, dt_065, dt_066, dt_067, dt_068, dt_069,
  dt_070, dt_071, dt_072, dt_073, dt_074, dt_075, dt_076, dt_077, dt_078, dt_079,
  dt_080, dt_081, dt_082, dt_083, dt_084, dt_085, dt_086, dt_087, dt_088, dt_089,
  dt_090, dt_091, dt_092, dt_093, dt_094, dt_095, dt_096, dt_097, dt_098, dt_099,
  dt_100, dt_101, dt_102, dt_103, dt_104, dt_105, dt_106, dt_107, dt_108, dt_109,
  dt_110, dt_111, dt_112, dt_113, dt_114,
};

void getDictionaryWord(uint16_t index) {
  // return tokiDictionary[index][0];
  strcpy_P(currentWord, (char *)pgm_read_ptr(&(dictionaryWords[index])));
}

void getDictionaryTranslation(uint16_t index) {
  // return tokiDictionary[index][1];
  strcpy_P(currentTranslation, (char *)pgm_read_ptr(&(dictionaryTranslations[index])));
}
