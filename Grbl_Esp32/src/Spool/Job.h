#pragma once

namespace Core {

    class Job {
    private:

    public:
        Job();
        virtual ~Job();

        void execute();
    };
}