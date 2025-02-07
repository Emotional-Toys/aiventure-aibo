#ifndef TERK_PEER_COMMON_ICE
#define TERK_PEER_COMMON_ICE

#include <Ice/Identity.ice>
#include <peer/MRPLPeer.ice>

[["java:package:edu.cmu.ri.mrpl"]]
module TeRK
   {
   enum ImageFormat {ImageJPEG, ImageRGB24, ImageRGB32, ImageGray8, ImageYUV420P, ImageUnknown};
   enum AudioMode {AudioTone,AudioClip};
   enum LEDMode {LEDOn,LEDOff,LEDBlinking};
   enum MotorMode {MotorSpeedControl,MotorPositionControl,MotorOff};
   enum ServoMode {ServoMotorSpeedControl,ServoMotorPositionControl,ServoOff};

   sequence<bool> BooleanArray;
   sequence<byte> ByteArray;
   sequence<int> IntArray;
   sequence<short> ShortArray;
   ["java:type:java.util.ArrayList<String>"] sequence<string> StringArray;
   sequence<LEDMode> LEDModeArray;
   sequence<MotorMode> MotorModeArray;
   sequence<ServoMode> ServoModeArray;

   ["java:type:java.util.HashMap<String,Ice.Identity>"] dictionary<string,Ice::Identity> ProxyTypeIdToIdentityMap;

   // todo: remove this once we get rid of the old API commands
   exception GenericError
      {
      string reason;
      };

   exception TeRKException
      {
      string reason;
      };

   exception MotorException             extends TeRKException { };
   exception VideoException             extends TeRKException { };
   exception ReadOnlyPropertyException  extends TeRKException { };

   exception CommandException           extends TeRKException { };
   exception AudioCommandException      extends CommandException { };
   exception DigitalOutCommandException extends CommandException { };
   exception LEDCommandException        extends CommandException { };
   exception MotorCommandException      extends CommandException { };
   exception ServoCommandException      extends CommandException { };

   struct MotorBuffer
      {
      IntArray values;
      };

   sequence<MotorBuffer> MotorBufferArray;

   struct Image
      {
      int           height;
      int           width;
      int           frameNum;
      ImageFormat   format;
      ByteArray     data;
      };

   struct AnalogInState
      {
      ShortArray analogInValues;
      };

   struct BatteryState
      {
      int batteryVoltage;
      };

   struct ButtonState
      {
      BooleanArray buttonStates;       // buttons are not latched, so this just returns instantaneous value
      };

   struct DigitalInState
      {
      BooleanArray digitalInStates;
      };

   struct MotorState
      {
      IntArray       motorVelocities;
      IntArray       motorPositions;
      IntArray       motorCurrents;       // in mA
      IntArray       motorDutyCycles;
      BooleanArray   motorDone;
      };

   struct ServoState
      {
      IntArray servoPositions;
      };

   struct QwerkState
      {
      AnalogInState    analogIn;
      BatteryState     battery;
      ButtonState      button;
      DigitalInState   digitalIn;
      MotorState       motor;
      ServoState       servo;
      };

   struct AudioCommand
      {
      AudioMode mode;
      int       frequency;
      byte      amplitude;
      int       duration;     // milliseconds
      ByteArray sound;
      };

   struct DigitalOutCommand
      {
      BooleanArray digitalOutMask;
      BooleanArray digitalOutValues;
      };

   struct LEDCommand
      {
      BooleanArray ledMask;
      LEDModeArray ledModes;
      };

   struct MotorCommand
      {
      BooleanArray motorMask;
      MotorModeArray motorModes;
      IntArray motorPositions;
      IntArray motorVelocities;
      IntArray motorAccelerations;
      };

   struct ServoCommand
      {
      BooleanArray servoMask;
      ServoModeArray servoModes;
      IntArray servoPositions;
      IntArray servoSpeeds;
      };

   struct QwerkCommand
      {
      AudioCommand        audioCmd;
      DigitalOutCommand   digitalOutCmd;
      LEDCommand          ledCmd;
      MotorCommand        motorCmd;
      ServoCommand        servoCmd;
      };

   interface PropertyManager
      {
      ["ami"] string getProperty(string key);
      ["ami"] void setProperty(string key, string value) throws ReadOnlyPropertyException;
      ["ami"] StringArray getPropertyKeys();
      };

   interface AbstractCommandController extends PropertyManager { };

   sequence<AbstractCommandController*> AbstractCommandControllerProxyArray;

   interface AudioController extends AbstractCommandController
      {
      ["ami"] void execute(AudioCommand command) throws AudioCommandException;
      };

   interface DigitalOutController extends AbstractCommandController
      {
      ["ami"] void execute(DigitalOutCommand command) throws DigitalOutCommandException;
      };

   interface LEDController extends AbstractCommandController
      {
      ["ami"] void execute(LEDCommand command) throws LEDCommandException;
      };

   interface MotorController extends AbstractCommandController
      {
      ["ami"] MotorState execute(MotorCommand command) throws MotorCommandException;

      // Motor buffer commands
      ["ami"] int getFrequency();  // control periods per second
      ["ami"] void startMotorBufferRecord(BooleanArray motorMask) throws MotorException;
      ["ami"] idempotent void stopMotorBufferRecord(BooleanArray motorMask) throws MotorException;
      ["ami"] nonmutating MotorBufferArray getMotorBuffers(BooleanArray motorMask) throws MotorException;
      ["ami"] idempotent void setMotorBuffer(BooleanArray motorMask, MotorBufferArray motorBuffers) throws MotorException;
      ["ami"] void playMotorBuffer(BooleanArray motorMask) throws MotorException;

      // Old API (todo: remove these eventually)
      void driveForward();
      void driveBack();
      void spinLeft();
      void spinRight();
      idempotent void setMotorVelocities(IntArray motorValues) throws GenericError;
      void stopMotors();
      };

   interface ServoController extends AbstractCommandController
      {
      ["ami"] ServoState execute(ServoCommand command) throws ServoCommandException;

      // Old API (todo: remove these eventually)
      void cameraTiltUp();
      void cameraTiltDown();
      void cameraPanLeft();
      void cameraPanRight();
      void setServoPositions(IntArray servoPositions);
      void setServoVelocities(IntArray servoVelocities);
      void stopServos();
      };

   interface VideoStreamerClient
      {
      void newFrame(Image frame);
      };

   interface VideoStreamerServer extends AbstractCommandController
      {
      // for push model
      ["ami"] idempotent void startVideoStream() throws VideoException;
      ["ami"] idempotent void stopVideoStream() throws VideoException;

      // for pull model
      ["ami"] Image getFrame(int frameNumber) throws VideoException;
      };

   interface TerkUser extends peer::ConnectionEventHandler, PropertyManager
      {
      // Returns a map which specifies all the services that this TerkUser supports. The
      // map associates proxy types (i.e. the result of .ice_getId()) with the private proxy
      // identities that the user used to register the services with the relay (via
      // the registerProxy() or registerProxies() method).  Peers call this method to see which
      // commands the user supports so they know what kind of commands they can send to the peer.
      // Peers will typically use the private identities in the returned map to fetch proxies
      // from the relay via the getPeerProxy() or getPeerProxies() methods.
      ["ami"] nonmutating ProxyTypeIdToIdentityMap getSupportedServices();
      };

   interface Qwerk extends TerkUser
      {
      ["ami"] nonmutating QwerkState getState();
      ["ami"] QwerkState execute(QwerkCommand command) throws CommandException;
      ["ami"] idempotent QwerkState emergencyStop();

      // deprecated, use getSupportedServices() instead
      ["ami"] nonmutating ProxyTypeIdToIdentityMap getCommandControllerTypeToProxyIdentityMap();
      };

   // todo: eventually change the client from Is-A to Has-A for the VideoStreamerClient interface
   interface TerkClient extends TerkUser, VideoStreamerClient
      {
      };
   };

#endif
