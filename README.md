# Qstring
These are string manipulation utilities for C++ aiming for ease of use and intuitive interface. Please see the unit test for some examples. Sample usage as follows.
auto qs = Qstring();
// similar to python's 'FirstabcSecondabcThird'.split('abc')
qs("FirstabcSecondabcThird").split("abc");
// similar to python's 'First,Second,Third'.split(',')
qs("First,Second,Third").split(',');
//similar to python's 'first second third'.split()
qs("first second third").split();

//similar to python's '   abc   '.strip()
qs("   abc   ").strip();
// result would be "XYZabcUVW"
qs("abcXYZabcUVWabc").strip("abc");
