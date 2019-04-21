TARGET = maslab_cars

ADDRESS = build/release/

LIBS = -lwiringPi -lstdc++ -lopencv_imgcodecs -lraspicam -lraspicam_cv  -lopencv_core -lopencv_highgui -lopencv_imgproc

.PHONY: all clear

all: $(TARGET)

clean:
	@rm -rf $(TARGET)

$(TARGET): $(ADDRESS)main.cpp clean
	@g++ $< -o $@ $(LIBS)
