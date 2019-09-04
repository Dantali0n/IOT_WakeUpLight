# MockDuino

Library to mock Arduino environment and functionality of both core Arduino and some common libraries.

**MockDuino maturity level:** Nasty dirt hack.

Ideal situation: read actual libraries and Arduino code and parse it with some kind of parser to automatically generate a mock library stub with a common interface for individual classes and functions.

```c++
// Dummy code ahead
void testCase() {
	analogReadPush(255); // push 255 unto the return stack for function analogRead()
	analogReadPush(128);
	analogRead(); // will now return 255
	analogRead(); // will now return 128
	analogRead(); // will now return 0
}
```

```c++
// More dummy code ahead
void anothertestCase(){
	Serial.println("example");
	REQUIRE(Serial.println.assertCalledWith("example") == true);
}
```