import cv2
import datetime

cam = cv2.VideoCapture(0)


def nothing(x):
    pass


threshold1 = 100
threshold2 = 100
cv2.namedWindow("trackbars")
cv2.createTrackbar("threshold1", "trackbars", threshold1, 500, nothing)
cv2.createTrackbar("threshold2", "trackbars", threshold2, 500, nothing)

while True:
    ret, frame = cam.read()
    flipped = cv2.flip(frame, 1)

    start_time = datetime.datetime.now()
    gray = cv2.cvtColor(flipped, cv2.COLOR_BGR2GRAY)

    Canny = cv2.Canny(gray, cv2.getTrackbarPos('threshold1', 'trackbars'),
                      cv2.getTrackbarPos('threshold2', 'trackbars'))

    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
    morph = cv2.morphologyEx(Canny, cv2.MORPH_CLOSE, kernel)
    contours, find = cv2.findContours(morph, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contoursDraw = cv2.drawContours(flipped, contours, -1, (0, 0, 255))
    end_time = datetime.datetime.now()

    if not ret:
        print("failed to grab frame")
        break
    cv2.imshow("Webcam", contoursDraw)

    time_diff = (end_time - start_time)
    execution_time = time_diff.total_seconds() * 1000000
    print(f'Czas przetwarzania obrazu {int(execution_time)} mikrosekund')

    k = cv2.waitKey(1)
    if k % 256 == 27:
        # ESC pressed
        print("Escape hit, closing...")
        break

cam.release()
cv2.destroyAllWindows()
