// ============================================================
// Batched Noncompliant Code - Cppcheck
// Compiler:  2.20.0 (latest)
// Flags:     --enable=all
// Tool:      Cppcheck
// ============================================================

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <syslog.h>
#include <iostream>
#include <new>

// ------------------------------------------------------------
// STD-001 | INT31-C | Integer conversion - data loss
// ------------------------------------------------------------
void test_std001(void) {
  unsigned long int u_a = ULONG_MAX;
  signed char sc;
  sc = (signed char)u_a; /* Cast eliminates warning */
  (void)sc;
}

// ------------------------------------------------------------
// STD-002 | EXP53-CPP | Uninitialized memory read
// ------------------------------------------------------------
void test_std002() {
  int i;
  std::cout << i;
}

// ------------------------------------------------------------
// STD-003 | STR50-CPP | String buffer - no bounds on input
// ------------------------------------------------------------
void test_std003() {
  char buf[12];
  std::cin >> buf;
}

// ------------------------------------------------------------
// STD-004 | FIO30-C | User input in format string (syslog)
// ------------------------------------------------------------
void test_std004(const char *user) {
  static const char msg_format[] = "%s cannot be authenticated.\n";
  size_t len = strlen(user) + sizeof(msg_format);
  char *msg = (char *)malloc(len);
  if (msg == NULL) { return; }
  int ret = snprintf(msg, len, msg_format, user);
  if (ret < 0 || ret >= (int)len) { free(msg); return; }
  syslog(LOG_INFO, msg); /* msg used directly as format string */
  free(msg);
}

// ------------------------------------------------------------
// STD-005 | MEM50-CPP | Access after free/delete
// ------------------------------------------------------------
struct S_std005 {
  void f() {}
};

void test_std005() noexcept(false) {
  S_std005 *s = new S_std005;
  delete s;
  s->f(); /* Use after delete */
}

// ------------------------------------------------------------
// STD-006 | PRE31-C | Side effect inside assert macro
// ------------------------------------------------------------
void test_std006(size_t index) {
  assert(index++ > 0); /* Side effect: index modified */
  (void)index;
}

// ------------------------------------------------------------
// STD-007 | ERR57-CPP | Resource leak on exception
// ------------------------------------------------------------
struct A_std007 {};
struct B_std007 {};

class C_std007 {
  A_std007 *a;
  B_std007 *b;
protected:
  void init() noexcept(false) {}
public:
  C_std007() : a(new A_std007()), b(new B_std007()) {
    init(); /* If init() throws, both a and b leak */
  }
};

// ------------------------------------------------------------
// STD-008 | FIO47-C | Wrong format specifiers (swapped %s/%d)
// ------------------------------------------------------------
void test_std008(void) {
  const char *error_msg = "Resource not available to user.";
  int error_type = 3;
  printf("Error (type %s): %d\n", error_type, error_msg); /* Swapped */
}

// ------------------------------------------------------------
// STD-009 | STR31-C | sprintf with unbounded %s - buffer overflow
// ------------------------------------------------------------
void test_std009(const char *name) {
  char filename[128];
  sprintf(filename, "%s.txt", name); /* name could exceed 123 chars */
}

// ------------------------------------------------------------
// STD-010 | EXP54-CPP | Object access outside its lifetime
// ------------------------------------------------------------
struct S_std010 {
  void mem_fn() {}
};

void test_std010() {
  S_std010 *s;   /* Uninitialized pointer */
  s->mem_fn();   /* Undefined behaviour */
}