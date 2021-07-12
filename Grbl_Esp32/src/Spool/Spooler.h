
namespace Core {

    class Spooler {
    private:
        

    public:
        Spooler();
        virtual ~Spooler();

        void start();
        void resume();
        void pause();
        void abort();

    };
}