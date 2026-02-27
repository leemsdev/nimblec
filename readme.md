---

# NimbleC

NimbleC is a small language that transpiles to C.

It is not a replacement for C. It is C — but with reduced friction.

The goal is to keep full C ABI compatibility while improving ergonomics and safety through syntax sugar and lightweight structural features.

The output of NimbleC is standard C, compiled with clang or gcc.

---

# Core Philosophy

* No semicolons
* Minimal boilerplate
* Deterministic execution
* No hidden runtime (beyond tiny helpers)
* C interop is first-class
* Every feature must justify itself

---

# v0.1 Feature Set

## 1. No Semicolons

Statements end at newline.

Generated C always includes semicolons.

---

## 2. Functions

### Syntax

```nimble
fn add(x int, y int) int {
    return x + y
}
```

* Parameters require explicit types.
* Single return value only (v0.1).
* `main` defaults to returning `int`.

---

## 3. Script Mode (No main Required)

Top-level statements are allowed.

Example:

```nimble
print("starting")

name := "lee"
age := 31

inp := parse_int(argv[1])
new_age := age + inp

print("your age will be {}", new_age)
```

If top-level statements exist, NimbleC generates:

```c
int main(int argc, char** argv) {
    ...
    return 0;
}
```

Rules:

* `argc` and `argv` are implicitly in scope in script files.
* `return` at top-level exits `main`.
* Top-level `defer` runs at script exit.

---

## 4. Variable Declaration (`:=`)

```nimble
x := 10
name := "lee"
```

Inference (v0.1):

* Integer literal → `int`
* Float literal → `double`
* String literal → `const char*`
* Function call → function return type

If type cannot be inferred, compilation fails.

---

## 5. Loops (No `while`)

There is only one looping keyword: `for`.

### Range Loop

```nimble
for i in 0..10 {
    print(i)
}
```

Desugars to:

```c
for (size_t i = 0; i < 10; i++) {
    ...
}
```

Rules:

* Upper bound is exclusive.
* `i` is scoped to the loop body.
* Default type is `size_t` (or `int`, design choice).

---

### Condition Loop

```nimble
for i < 10 {
    ...
}
```

Desugars to:

```c
for (; i < 10; ) {
    ...
}
```

If `i` does not already exist, it is created with default zero value (design choice).

You may instead require it to exist beforehand for clarity.

---

### Boolean Loop

```nimble
running := true
for running {
    ...
}
```

Desugars to:

```c
for (; running; ) {
    ...
}
```

---

### Infinite Loop

```nimble
for {
    ...
}
```

Desugars to:

```c
for (;;) {
    ...
}
```

---

No `while` keyword exists.

---

## 6. defer

### Syntax

```nimble
defer close(fd)
```

Rules:

* Only function calls allowed (v0.1).
* Executes in LIFO order.
* Runs on all return paths.

Implementation model:

All functions are transformed to use a single exit label.

Returns become:

```c
_nimble_result = expr;
goto _nimble_cleanup;
```

At cleanup:

* Execute defers in reverse order
* Free arena
* Return result

---

## 7. Per-Function Temporary Allocator

Each function implicitly creates a temporary arena allocator.

```nimble
buf := alloc(128)
```

Desugars to:

```c
void* buf = nimble_arena_alloc(&_arena, 128);
```

At function entry:

```c
nimble_arena _arena;
nimble_arena_init(&_arena);
```

At cleanup:

```c
nimble_arena_free(&_arena);
```

Rule:

Memory allocated via `alloc()` must not escape the function.

Returning it or storing it long-term is undefined behavior (v0.1). Future versions may enforce this.

---

## 8. Printing

Basic built-in:

```nimble
print("hello")
print("value is {}", x)
```

Compiles to `printf`.

Simple `{}` placeholder formatting supported.

---

## 9. C Interoperability

### Import

```nimble
import "stdio.h"
```

→

```c
#include <stdio.h>
```

### Extern

```nimble
extern fn printf(fmt *char, ...) int
```

→

```c
extern int printf(const char*, ...);
```

NimbleC uses the C ABI.

---

# Example

```nimble
print("starting")

for i in 0..5 {
    print(i)
}

running := true

for running {
    running = false
}
```

Compiles to predictable, clean C.

---
