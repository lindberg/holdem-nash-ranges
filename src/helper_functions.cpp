int FastMod(const int input, const int ceil) {
	// Apply the modulo operator only when needed
	// (i.e. when the input is greater than the ceiling).
	return input >= ceil ? input % ceil : input;
	// NB: the assumption here is that the numbers are positive.
}