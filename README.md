### Script to generate FSA transition diagram and associated RegEx

To run this cpp script you can use CLion, or run it through the terminal (assuming you have gcc/clang installed)

gcc:

``` bash
  g++ ToCAutomata.cpp -o temp && ./temp
```

Or with clang:

``` bash
  clang++ ToCAutomata.cpp -o temp && ./temp
```

Enter your IT login e.g. abcd036 (NO WHITESPACE)

Observe results, precisely telling you which state matches to what, as well as the regEx constructed using Kleene Theorem.
