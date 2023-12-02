#include <cassert>

class Parser {
private:
    const char* const start;

    const char* current;
    const char* marker {nullptr};

public:
    Parser(const char* start) : start(start), current(start) {

    }

    bool consume(char c) {
        if (c != '\0' && *current == c) {
            return ++current;
        }

        return false;
    }

    /**
     * Consume an exact ma
     * tch of characters in the order they are provided.
     * Position will only be advanced if all characters were consumed.
     *
     * @param match c string
     * @return true if matched; false otherwise.
     */
    bool consume(const char* match) {
        const char* c = current;
        while (*c != '\0' && *match != '\0' && *c == *match) {
            ++c;
            ++match;
        }

        if (*match == '\0') {
            current = c;
            return true;
        } else {
            return false;
        }
    }

    char read() {
        if (*current == '\0')
            return *current;
        else
            return *(current++);
    }

    bool readNewLine() {
        const char* c = current;
        if (*c == '\r' && *(c + 1) == '\n') {
            current = c + 2;
            return true;
        } else if (*c == '\n') {
            current = c + 1;
            return true;
        }

        return false;
    }

    bool readInt(int &i) {
        const char* c = current;
        while (*c >= '0' && *c <= '9') {
            i = i * 10 + (*(c++) - '0');
        }

        if (c != current) {
            current = c;
            return true;
        } else {
            return false;
        }
    }

    [[nodiscard]] bool eof() const {
        return *current == '\0';
    }

    /**
     * Places a marked at the current position.
     */
    void mark() {
        marker = current;
    }

    /**
     * Resets to the previous placed marker.
     */
    void resetToMarker() {
        assert(marker != nullptr);
        current = marker;
    }

    /**
     * Resets to the beginning
     */
    void resetToBeginning() {
        current = start;
    }
};