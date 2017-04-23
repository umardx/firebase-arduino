#include <FirebaseArduino.h>

int main() {
	Firebase.begin("traffic-22311.firebaseio.com", "df9u8Mn6Yoca61q1iM6hOPLZ5zD9e8uQiMdhZyX2");
	Firebase.set("pushbutton", 0);
	Firebase.set("sunlight", 0);
	Firebase.set("redlight", 0);
	Firebase.set("cooldown", 0);
	Firebase.set("brrr", 0);
}