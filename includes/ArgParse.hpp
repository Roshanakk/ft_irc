
class ArgParse {
  public:
    static void parsePort();
    static void parsePass();

  private:
    // Making the constructor private so that the class can't be instantiated
    ArgParse();
    ~ArgParse();
}