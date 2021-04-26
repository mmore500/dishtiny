template <typename T>
class A {
  T b;

  void E() {
    b++;
  }
};

template <typename T>
void C(T d) {
  d += 5;
}

int main() {
  A<int> a;
}
