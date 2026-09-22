import cv2
import mediapipe as mp
import face_recognition
import serial
import time
import os
import warnings

warnings.filterwarnings("ignore")


# =====================================================
# SETTINGS
# =====================================================

folder = r"C:\Users\Smartims\Desktop\authorized_faces"

ARDUINO_PORT = "COM11"
BAUD_RATE = 9600


# =====================================================
# CONNECT TO ARDUINO 1
# =====================================================

try:

    arduino = serial.Serial(
        ARDUINO_PORT,
        BAUD_RATE,
        timeout=1
    )

    time.sleep(2)

    print("Arduino 1 connected:", ARDUINO_PORT)

except serial.SerialException as e:

    print("ERROR: Could not open", ARDUINO_PORT)
    print(e)
    print()
    print("Close Arduino Serial Monitor/Serial Plotter")
    print("and try again.")

    raise SystemExit


# =====================================================
# LOAD AUTHORIZED FACES
# =====================================================

known_face_encodings = []
known_face_names = []


for filename in os.listdir(folder):

    if filename.lower().endswith(
        (".jpg", ".jpeg", ".png")
    ):

        image_path = os.path.join(
            folder,
            filename
        )


        image = face_recognition.load_image_file(
            image_path
        )


        encodings = face_recognition.face_encodings(
            image
        )


        if len(encodings) == 1:

            known_face_encodings.append(
                encodings[0]
            )

            known_face_names.append(
                os.path.splitext(filename)[0]
            )

            print("Loaded:", filename)


        else:

            print(
                "Skipped:",
                filename,
                "- exactly one face required"
            )


print(
    "Authorized faces:",
    len(known_face_encodings)
)


# =====================================================
# MEDIAPIPE HANDS
# =====================================================

mp_hands = mp.solutions.hands

mp_draw = mp.solutions.drawing_utils


hands = mp_hands.Hands(
    static_image_mode=False,
    max_num_hands=1,
    min_detection_confidence=0.6,
    min_tracking_confidence=0.6
)


# =====================================================
# CAMERA
# =====================================================

cap = cv2.VideoCapture(0)


if not cap.isOpened():

    print("ERROR: Camera could not be opened.")

    arduino.close()

    raise SystemExit


# =====================================================
# LAST COMMAND
# =====================================================

last_command = ""


def send_command(command):

    global last_command


    if command != last_command:

        arduino.write(
            (command + "\n").encode()
        )

        print("Sent:", command)

        last_command = command


# =====================================================
# GESTURE DETECTION
# =====================================================

def get_gesture(hand_landmarks):

    index_up = (
        hand_landmarks.landmark[8].y
        <
        hand_landmarks.landmark[6].y
    )


    middle_up = (
        hand_landmarks.landmark[12].y
        <
        hand_landmarks.landmark[10].y
    )


    ring_up = (
        hand_landmarks.landmark[16].y
        <
        hand_landmarks.landmark[14].y
    )


    pinky_up = (
        hand_landmarks.landmark[20].y
        <
        hand_landmarks.landmark[18].y
    )


    # ================================================
    # GESTURE 1
    # INDEX FINGER ONLY
    # ================================================

    if (
        index_up
        and not middle_up
        and not ring_up
        and not pinky_up
    ):

        return "EW"


    # ================================================
    # GESTURE 2
    # OPEN PALM
    # ================================================

    if (
        index_up
        and middle_up
        and ring_up
        and pinky_up
    ):

        return "NS"


    return "NONE"


# =====================================================
# MAIN LOOP
# =====================================================

while True:

    ret, frame = cap.read()


    if not ret:

        print("Camera frame error")

        break


    frame = cv2.flip(frame, 1)


    # =================================================
    # FACE RECOGNITION
    # =================================================

    small_frame = cv2.resize(
        frame,
        (0, 0),
        fx=0.25,
        fy=0.25
    )


    rgb_small = cv2.cvtColor(
        small_frame,
        cv2.COLOR_BGR2RGB
    )


    face_locations = (
        face_recognition.face_locations(
            rgb_small
        )
    )


    face_encodings = (
        face_recognition.face_encodings(
            rgb_small,
            face_locations
        )
    )


    authorized = False

    person_name = "Unknown"


    for face_encoding in face_encodings:

        if len(known_face_encodings) == 0:

            break


        matches = (
            face_recognition.compare_faces(
                known_face_encodings,
                face_encoding,
                tolerance=0.5
            )
        )


        if True in matches:

            index = matches.index(True)

            authorized = True

            person_name = (
                known_face_names[index]
            )

            break


    # =================================================
    # AUTHORIZED PERSON
    # =================================================

    if authorized:

        cv2.putText(
            frame,
            "AUTHORIZED: " + person_name,
            (20, 40),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.7,
            (0, 255, 0),
            2
        )


        # =============================================
        # HAND DETECTION
        # =============================================

        rgb_frame = cv2.cvtColor(
            frame,
            cv2.COLOR_BGR2RGB
        )


        result = hands.process(
            rgb_frame
        )


        gesture = "NONE"


        if result.multi_hand_landmarks:

            hand = (
                result.multi_hand_landmarks[0]
            )


            gesture = get_gesture(hand)


            mp_draw.draw_landmarks(
                frame,
                hand,
                mp_hands.HAND_CONNECTIONS
            )


        # =============================================
        # INDEX FINGER
        # =============================================

        if gesture == "EW":

            cv2.putText(
                frame,
                "INDEX -> EAST + WEST GREEN",
                (20, 80),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.6,
                (0, 255, 0),
                2
            )


            send_command("EW")


        # =============================================
        # OPEN PALM
        # =============================================

        elif gesture == "NS":

            cv2.putText(
                frame,
                "PALM -> NORTH + SOUTH GREEN",
                (20, 80),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.6,
                (0, 255, 0),
                2
            )


            send_command("NS")


        # =============================================
        # NO GESTURE
        # =============================================

        else:

            cv2.putText(
                frame,
                "NO GESTURE -> AUTO",
                (20, 80),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.6,
                (0, 255, 255),
                2
            )


            send_command("AUTO")


    # =================================================
    # UNAUTHORIZED
    # =================================================

    else:

        cv2.putText(
            frame,
            "UNAUTHORIZED",
            (20, 40),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.7,
            (0, 0, 255),
            2
        )


        send_command("AUTO")


    # =================================================
    # SHOW CAMERA
    # =================================================

    cv2.imshow(
        "Face Authentication + Gesture Control",
        frame
    )


    # =================================================
    # QUIT
    # =================================================

    key = cv2.waitKey(1) & 0xFF


    if key == ord("q"):

        send_command("AUTO")

        break


# =====================================================
# CLEANUP
# =====================================================

cap.release()

hands.close()

cv2.destroyAllWindows()

arduino.close()

print("System stopped.")