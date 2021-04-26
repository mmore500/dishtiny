template <typename T>
class A {
  T b;

  void E() {/*_FORCE_COVER_START_*/
    b++;
  }/*_FORCE_COVER_END_*/
};

template <typename T>
void C(T d) {/*_FORCE_COVER_START_*/
  d += 5;
}/*_FORCE_COVER_END_*/

int main() {
  int a = 0;
}
