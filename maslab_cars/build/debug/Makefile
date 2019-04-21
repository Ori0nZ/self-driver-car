TARGET = debug

ADDRESS = 

LIBS = -lwiringPi -lstdc++ -lopencv_imgcodecs -lraspicam -lraspicam_cv  -lopencv_core -lopencv_highgui -lopencv_imgproc

.PHONY: all clear

all: $(TARGET)

clean:
	@rm -rf $(TARGET)

$(TARGET): $(ADDRESS)debug.cpp clean
	@g++ $< -o $@ $(LIBS)

