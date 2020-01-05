# QtQmlDataCommunicationEx
All communication types between Qml and Qt components
QML Applications:
-----------------
QML is a declarative language that allows user interfaces to be described in terms of their visual components and how they interact and relate with one another. It is a highly readable language that was designed to enable components to be interconnected in a dynamic manner, and it allows components to be easily reused and customized within a user interface. Using the QtQuick module, designers and developers can easily build fluid animated user interfaces in QML, and have the option of connecting these user interfaces to any back-end C++ libraries.

What is QML?
------------
QML is a user interface specification and programming language. It allows developers and designers alike to create highly performant, fluidly animated and visually appealing applications. QML offers a highly readable, declarative, JSON-like syntax with support for imperative JavaScript expressions combined with dynamic property bindings.

The QML language and engine infrastructure is provided by the Qt QML module. For in-depth information about the QML language, please see the Qt QML module documentation.

What is Qt Quick?
----------------
Qt Quick is the standard library of types and functionality for QML. It includes visual types, interactive types, animations, models and views, particle effects and shader effects. A QML application developer can get access to all of that functionality with a single import statement.

The QtQuick QML library is provided by the Qt Quick module. For in-depth information about the various QML types and other functionality provided by Qt Quick, please see the Qt Quick module documentation.

To use the new QQml* and QQuick* classes in Qt 5, link against the approprate module from your qmake .pro file. For example the following will link against both the Qt QML and Qt Quick modules:

QT += qml quick
Required header files can then be included:

#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>

QT QML and Qt Quick module:
-------------------------
QML:(https://doc.qt.io/qt-5/qtqml-index.html)
----
The Qt QML module provides a framework for developing applications and libraries with the QML language. It defines and implements the language and engine infrastructure, and provides an API to enable application developers to extend the QML language with custom types and integrate QML code with JavaScript and C++. The Qt QML module provides both a QML API and a C++ API.

Note that while the Qt QML module provides the language and infrastructure for QML applications, the Qt Quick module provides many visual components, model-view support, an animation framework, and much more for building user interfaces.

import QtQml 2.14
QT += qml

Quick:(https://doc.qt.io/qt-5/qtquick-index.html)
------
The Qt Quick module is the standard library for writing QML applications. While the Qt QML module provides the QML engine and language infrastructure, the Qt Quick module provides all the basic types necessary for creating user interfaces with QML. It provides a visual canvas and includes types for creating and animating visual components, receiving user input, creating data models and views and delayed object instantiation.

The Qt Quick module provides both a QML API which supplies QML types for creating user interfaces with the QML language, and a C++ API for extending QML applications with C++ code.

Note: A set of Qt Quick-based UI controls is also available to create user interfaces. See Qt Quick Controls for more information.

Interacting with QML Objects from C++:(https://doc.qt.io/qt-5/qtqml-cppintegration-interactqmlfromcpp.html)
-------------------------------------
All QML object types are QObject-derived types, whether they are internally implemented by the engine or defined by third-party sources. This means the QML engine can use the Qt Meta Object System to dynamically instantiate any QML object type and inspect the created objects.

This is useful for creating QML objects from C++ code, whether to display a QML object that can be visually rendered, or to integrate non-visual QML object data into a C++ application. Once a QML object is created, it can be inspected from C++ in order to read and write to properties, invoke methods and receive signal notifications.

Loading QML Objects from C++:
-------------------------------
A QML document can be loaded with QQmlComponent or QQuickView. QQmlComponent loads a QML document as a C++ object that can then be modified from C++ code. QQuickView also does this, but as QQuickView is a QWindow-derived class, the loaded object will also be rendered into a visual display; QQuickView is generally used to integrate a displayable QML object into an application's user interface.

For example, suppose there is a MyItem.qml file that looks like this:

import QtQuick 2.0

Item {
    width: 100; height: 100
}
This QML document can be loaded with QQmlComponent or QQuickView with the following C++ code. Using a QQmlComponent requires calling QQmlComponent::create() to create a new instance of the component, while a QQuickView automatically creates an instance of the component, which is accessible via QQuickView::rootObject():

// Using QQmlComponent
QQmlEngine engine;
QQmlComponent component(&engine,
        QUrl::fromLocalFile("MyItem.qml"));
QObject *object = component.create();
...
delete object;

// Using QQuickView
QQuickView view;
view.setSource(QUrl::fromLocalFile("MyItem.qml"));
view.show();
QObject *object = view.rootObject();

This object is the instance of the MyItem.qml component that has been created. You can now modify the item's properties using QObject::setProperty() or QQmlProperty::write():

object->setProperty("width", 500);
QQmlProperty(object, "width").write(500);

The difference between QObject::setProperty() and QQmlProperty::write() is that the latter will also remove the binding in addition to setting the property value. For example, suppose the width assignment above had been a binding to height:

width: height
If the height of the Item changed after the object->setProperty("width", 500) call, the width would be updated again, as the binding remains active. However, if the height changes after the QQmlProperty(object, "width").write(500) call, the width will not be changed, as the binding does not exist anymore.

Alternatively, you can cast the object to its actual type and call methods with compile-time safety. In this case the base object of MyItem.qml is an Item, which is defined by the QQuickItem class:

QQuickItem *item = qobject_cast<QQuickItem*>(object);
item->setWidth(500);
You can also connect to any signals or call methods defined in the component using QMetaObject::invokeMethod() and QObject::connect(). See Invoking QML Methods and Connecting to QML Signals below for further details.


Invoking QML Methods:
------------------------
Invoking QML Methods
All QML methods are exposed to the meta-object system and can be called from C++ using QMetaObject::invokeMethod(). You can specify types for the parameters and the return value after the colon character, as shown in the code snippet below. This can be useful, for example, when you want to connect a signal in C++ with a certain signature to a QML-defined method. If you omit the types, the C++ signature will use QVariant.

Here is a C++ application that calls a QML method using QMetaObject::invokeMethod():

// MyItem.qml
import QtQuick 2.0

Item {
    function myQmlFunction(msg: string) : string {
        console.log("Got message:", msg)
        return "some return value"
    }
}

// main.cpp - c++
QQmlEngine engine;
QQmlComponent component(&engine, "MyItem.qml");
QObject *object = component.create();

QString returnedValue;
QString msg = "Hello from C++";
QMetaObject::invokeMethod(object, "myQmlFunction",
        Q_RETURN_ARG(QString, returnedValue),
        Q_ARG(QString, msg));
//https://doc.qt.io/qt-5/qmetaobject.html#invokeMethod
//NOTE: QMetaObject::invokeMethod is thread safe
qDebug() << "QML function returned:" << returnedValue;
delete object;

Notice the parameter and return type specified after the colon. You can use basic types and object types as type names.

If the type is omitted in QML, then you must specify QVariant as type with Q_RETURN_ARG() and Q_ARG() when calling QMetaObject::invokeMethod.

Connecting to QML Signals:
-------------------------
All QML signals are automatically available to C++, and can be connected to using QObject::connect() like any ordinary Qt C++ signal. In return, any C++ signal can be received by a QML object using signal handlers.

Here is a QML component with a signal named qmlSignal that is emitted with a string-type parameter. This signal is connected to a C++ object's slot using QObject::connect(), so that the cppSlot() method is called whenever the qmlSignal is emitted:

// MyItem.qml
import QtQuick 2.0

Item {
    id: item
    width: 100; height: 100

    signal qmlSignal(msg: string)

    MouseArea {
        anchors.fill: parent
        onClicked: item.qmlSignal("Hello from QML")
    }
}

class MyClass : public QObject
{
    Q_OBJECT
public slots:
    void cppSlot(const QString &msg) {
        qDebug() << "Called the C++ slot with message:" << msg;
    }
};

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQuickView view(QUrl::fromLocalFile("MyItem.qml"));
    QObject *item = view.rootObject();

    MyClass myClass;
    QObject::connect(item, SIGNAL(qmlSignal(QString)),
                     &myClass, SLOT(cppSlot(QString)));

    view.show();
    return app.exec();
}

A QML object type in a signal parameter is translated to a pointer to the class in C++:

// MyItem.qml
import QtQuick 2.0

Item {
    id: item
    width: 100; height: 100

    signal qmlSignal(anObject: Item)

    MouseArea {
        anchors.fill: parent
        onClicked: item.qmlSignal(item)
    }
}

class MyClass : public QObject
{
    Q_OBJECT
public slots:
    void cppSlot(QQuickItem *item) {
       qDebug() << "Called the C++ slot with item:" << item;

       qDebug() << "Item dimensions:" << item->width()
                << item->height();
    }
};

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQuickView view(QUrl::fromLocalFile("MyItem.qml"));
    QObject *item = view.rootObject();

    MyClass myClass;
    QObject::connect(item, SIGNAL(qmlSignal(QVariant)),
                     &myClass, SLOT(cppSlot(QVariant)));

    view.show();
    return app.exec();
}

Dynamic Bindings:
---------------------

Property bindings are a fundamental feature of QML. Typically, a property is initialized with its binding. However, the Binding type and Qt.binding() function allows the client to dynamically bind properties from any object at run-time, and modify the binding target when required (or when it becomes available).

Property Binding:
----------------------
An object's property can be assigned a static value which stays constant until it is explicitly assigned a new value. However, to make the fullest use of QML and its built-in support for dynamic object behaviors, most QML objects use property bindings.

Property bindings are a core feature of QML that lets developers specify relationships between different object properties. When a property's dependencies change in value, the property is automatically updated according to the specified relationship.

Behind the scenes, the QML engine monitors the property's dependencies (that is, the variables in the binding expression). When a change is detected, the QML engine re-evaluates the binding expression and applies the new result to the property.

Example:
------------
Rectangle {
    width: 200; height: 200

    Rectangle {
        width: 100
        height: parent.height
        color: "blue"
    }
}
Whenever the height of the parent rectangle changes, the height of the blue rectangle automatically updates to be of the same value.

A binding can contain any valid JavaScript expression or statement, as QML uses a standards compliant JavaScript engine. Bindings can access object properties, call methods and use built-in JavaScript objects such as Date and Math. Below are other possible bindings for the previous example:

height: parent.height / 2

height: Math.min(parent.width, parent.height)

height: parent.height > 100 ? parent.height : parent.height/2

height: {
    if (parent.height > 100)
        return parent.height
    else
        return parent.height / 2
}
Creating Property Bindings from JavaScript
A property with a binding is automatically updated as necessary. However, if the property is later assigned a static value from a JavaScript statement, the binding will be removed.

For example, the Rectangle below initially ensures that its height is always twice its width. However, when the space key is pressed, the current value of width*3 will be assigned to height as a static value. After that, the height will remain fixed at this value, even if the width changes. The assignment of the static value removes the binding.

import QtQuick 2.0

Rectangle {
    width: 100
    height: width * 2

    focus: true
    Keys.onSpacePressed: {
        height = width * 3
    }
}
If the intention is to give the rectangle a fixed height and stop automatic updates, then this is not a problem. However, if the intention is to establish a new relationship between width and height, then the new binding expression must be wrapped in the Qt.binding() function instead:

import QtQuick 2.0

Rectangle {
    width: 100
    height: width * 2

    focus: true
    Keys.onSpacePressed: {
        height = Qt.binding(function() { return width * 3 })
    }
}
Now, after the space key is pressed, the rectangle's height will continue auto-updating to always be three times its width.

Debugging overwriting of bindings:
---------------------------------
A common cause of bugs in QML applications is accidentally overwriting bindings with static values from JavaScript statements. To help developers track down problems of this kind, the QML engine is able to emit messages whenever a binding is lost due to imperative assignments.

In order to generate such messages, you need to enable the informational output for the qt.qml.binding.removal logging category, for instance by calling:

QLoggingCategory::setFilterRules(QStringLiteral("qt.qml.binding.removal.info=true"));
Please refer to the QLoggingCategory documentation for more information about enabling output from logging categories.

Note that is perfectly reasonable in some circumstances to overwrite bindings. Any message generated by the QML engine should be treated as a diagnostic aid, and not necessarily as evidence of a problem without further investigation.

Using this with Property Binding:
---------------------------------
When creating a property binding from JavaScript, the this keyword can be used to refer to the object which receives the binding. This is helpful for resolving ambiguities with property names.

For example, the Component.onCompleted handler below is defined within the scope of the Item. In this scope, width refers to the Item's width, not the Rectangle's width. To bind the Rectangle's height to its own width, the binding expression must explicitly refer to this.width (or alternatively, rect.width):

Item {
    width: 500
    height: 500

    Rectangle {
        id: rect
        width: 100
        color: "yellow"
    }

    Component.onCompleted: {
        rect.height = Qt.binding(function() { return this.width * 2 })
        console.log("rect.height = " + rect.height) // prints 200, not 1000
    }
}
Note: The value of this is not defined outside of property bindings. See JavaScript Environment Restrictions for details.

height: someMethodThatReturnsHeight()

Property Attributes:(https://doc.qt.io/qt-5/qtqml-syntax-objectattributes.html)
----------------------
A property is an attribute of an object that can be assigned a static value or bound to a dynamic expression. A property's value can be read by other objects. Generally it can also be modified by another object, unless a particular QML type has explicitly disallowed this for a specific property.

Defining Property Attributes:
------------------------------
A property may be defined for a type in C++ by registering a Q_PROPERTY of a class which is then registered with the QML type system. Alternatively, a custom property of an object type may be defined in an object declaration in a QML document with the following syntax:

[default] property <propertyType> <propertyName>
In this way an object declaration may expose a particular value to outside objects or maintain some internal state more easily.

Property names must begin with a lower case letter and can only contain letters, numbers and underscores. JavaScript reserved words are not valid property names. The default keyword is optional, and modifies the semantics of the property being declared. See the upcoming section on default properties for more information about the default property modifier.

Declaring a custom property implicitly creates a value-change signal for that property, as well as an associated signal handler called on<PropertyName>Changed, where <PropertyName> is the name of the property, with the first letter capitalized.

For example, the following object declaration defines a new type which derives from the Rectangle base type. It has two new properties, with a signal handler implemented for one of those new properties:

Rectangle {
    property color previousColor
    property color nextColor
    onNextColorChanged: console.log("The next color will be: " + nextColor.toString())
}
Valid Types in Custom Property Definitions
Any of the QML Basic Types aside from the enumeration type can be used as custom property types. For example, these are all valid property declarations:

Item {
    property int someNumber
    property string someString
    property url someUrl
}
(Enumeration values are simply whole number values and can be referred to with the int type instead.)

Some basic types are provided by the QtQuick module and thus cannot be used as property types unless the module is imported. See the QML Basic Types documentation for more details.

Note the var basic type is a generic placeholder type that can hold any type of value, including lists and objects:

property var someNumber: 1.5
property var someString: "abc"
property var someBool: true
property var someList: [1, 2, "three", "four"]
property var someObject: Rectangle { width: 100; height: 100; color: "red" }
Additionally, any QML object type can be used as a property type. For example:

property Item someItem
property Rectangle someRectangle
This applies to custom QML types as well. If a QML type was defined in a file named ColorfulButton.qml (in a directory which was then imported by the client), then a property of type ColorfulButton would also be valid.

Assigning Values to Property Attributes
The value of a property of an object instance may be specified in two separate ways:

a value assignment on initialization
an imperative value assignment
In either case, the value may be either a static value or a binding expression value.

Value Assignment on Initialization
The syntax for assigning a value to a property on initialization is:

<propertyName> : <value>
An initialization value assignment may be combined with a property definition in an object declaration, if desired. In that case, the syntax of the property definition becomes:

[default] property <propertyType> <propertyName> : <value>
An example of property value initialization follows:

import QtQuick 2.0

Rectangle {
    color: "red"
    property color nextColor: "blue" // combined property declaration and initialization
}
Imperative Value Assignment
An imperative value assignment is where a property value (either static value or binding expression) is assigned to a property from imperative JavaScript code. The syntax of an imperative value assignment is just the JavaScript assignment operator, as shown below:

[<objectId>.]<propertyName> = value
An example of imperative value assignment follows:

import QtQuick 2.0

Rectangle {
    id: rect
    Component.onCompleted: {
        rect.color = "red"
    }
}
//https://doc.qt.io/qt-5/qobject.html#Q_PROPERTY
Q_PROPERTY(...)
This macro is used for declaring properties in classes that inherit QObject. Properties behave like class data members, but they have additional features accessible through the Meta-Object System.

Q_PROPERTY(type name
           (READ getFunction [WRITE setFunction] |
            MEMBER memberName [(READ getFunction | WRITE setFunction)])
           [RESET resetFunction]
           [NOTIFY notifySignal]
           [REVISION int]
           [DESIGNABLE bool]
           [SCRIPTABLE bool]
           [STORED bool]
           [USER bool]
           [CONSTANT]
           [FINAL])
The property name and type and the READ function are required. The type can be any type supported by QVariant, or it can be a user-defined type. The other items are optional, but a WRITE function is common. The attributes default to true except USER, which defaults to false.

For example:

Q_PROPERTY(QString title READ title WRITE setTitle USER true)
For more details about how to use this macro, and a more detailed example of its use, see the discussion on Qt's Property System.


Exposing Attributes of C++ Types to QML:(https://doc.qt.io/qt-5/qtqml-cppintegration-exposecppattributes.html)
----------------------------------------
QML can easily be extended with functionality defined in C++ code. Due to the tight integration of the QML engine with the Qt meta-object system, any functionality that is appropriately exposed by a QObject-derived class is accessible from QML code. This enables C++ data and functions to be accessible directly from QML, often with little or no modification.

The QML engine has the ability to introspect QObject instances through the meta-object system. This means any QML code can access the following members of an instance of a QObject-derived class:

Properties
Methods (providing they are public slots or flagged with Q_INVOKABLE)
Signals
(Additionally, enums are available if they have been declared with Q_ENUMS. See Data Type Conversion Between QML and C++ for more details.)

In general, these are accessible from QML regardless of whether a QObject-derived class has been registered with the QML type system. However, if a class is to be used in a way that requires the engine to access additional type information — for example, if the class itself is to be used as a method parameter or property, or if one of its enum types is to be used in this way — then the class may need to be registered.

Also note that a number of the important concepts covered in this document are demonstrated in the Writing QML Extensions with C++ tutorial.
https://doc.qt.io/qt-5/qtqml-tutorials-extending-qml-example.html

Exposing Properties:
---------------------
A property can be specified for any QObject-derived class using the Q_PROPERTY() macro. A property is a class data member with an associated read function and optional write function.

All properties of a QObject-derived class are accessible from QML.

For example, below is a Message class with an author property. As specified by the Q_PROPERTY macro call, this property is readable through the author() method, and writable through the setAuthor() method:

class Message : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
public:
    void setAuthor(const QString &a) {
        if (a != m_author) {
            m_author = a;
            emit authorChanged();
        }
    }
    QString author() const {
        return m_author;
    }
signals:
    void authorChanged();
private:
    QString m_author;
};
If an instance of this class was set as a context property when loading a file named MyItem.qml from C++:

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQuickView view;
    Message msg;
    view.engine()->rootContext()->setContextProperty("msg", &msg);
    view.setSource(QUrl::fromLocalFile("MyItem.qml"));
    view.show();

    return app.exec();
}
Then, the author property could be read from MyItem.qml:

// MyItem.qml
import QtQuick 2.0

Text {
    width: 100; height: 100
    text: msg.author    // invokes Message::author() to get this value

    Component.onCompleted: {
        msg.author = "Jonah"  // invokes Message::setAuthor()
    }
}
For maximum interoperability with QML, any property that is writable should have an associated NOTIFY signal that is emitted whenever the property value has changed. This allows the property to be used with property binding, which is an essential feature of QML that enforces relationships between properties by automatically updating a property whenever any of its dependencies change in value.

In the above example, the associated NOTIFY signal for the author property is authorChanged, as specified in the Q_PROPERTY() macro call. This means that whenever the signal is emitted — as it is when the author changes in Message::setAuthor() — this notifies the QML engine that any bindings involving the author property must be updated, and in turn, the engine will update the text property by calling Message::author() again.

If the author property was writable but did not have an associated NOTIFY signal, the text value would be initialized with the initial value returned by Message::author() but would not be updated with any later changes to this property. In addition, any attempts to bind to the property from QML will produce a runtime warning from the engine.

Note: It is recommended that the NOTIFY signal be named <property>Changed where <property> is the name of the property. The associated property change signal handler generated by the QML engine will always take the form on<Property>Changed, regardless of the name of the related C++ signal, so it is recommended that the signal name follows this convention to avoid any confusion.

Notes on Use of Notify Signals:
----------------------------------
To prevent loops or excessive evaluation, developers should ensure that the property change signal is only emitted when the property value has actually changed. Also, if a property or group of properties is infrequently used, it is permitted to use the same NOTIFY signal for several properties. This should be done with care to ensure that performance doesn't suffer.

The presence of a NOTIFY signal does incur a small overhead. There are cases where a property's value is set at object construction time, and does not subsequently change. The most common case of this is when a type uses Grouped Properties, and the grouped property object is allocated once, and only freed when the object is deleted. In these cases, the CONSTANT attribute may be added to the property declaration instead of a NOTIFY signal.

The CONSTANT attribute should only be used for properties whose value is set, and finalized, only in the class constructor. All other properties that want to be used in bindings should have a NOTIFY signal instead.

Properties with Object Types:
---------------------------------
Object-type properties are accessible from QML providing that the object type has been appropriately registered with the QML type system.

For example, the Message type might have a body property of type MessageBody*:

class Message : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MessageBody* body READ body WRITE setBody NOTIFY bodyChanged)
public:
    MessageBody* body() const;
    void setBody(MessageBody* body);
};

class MessageBody : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE text NOTIFY textChanged)
// ...
}
Suppose the Message type was registered with the QML type system, allowing it to be used as an object type from QML code:

Message {
    // ...
}
If the MessageBody type was also registered with the type system, it would be possible to assign MessageBody to the body property of a Message, all from within QML code:

Message {
    body: MessageBody {
        text: "Hello, world!"
    }
}
Properties with Object-List Types:
--------------------------------------
Properties containing lists of QObject-derived types can also be exposed to QML. For this purpose, however, one should use QQmlListProperty rather than QList<T> as the property type. This is because QList is not a QObject-derived type, and so cannot provide the necessary QML property characteristics through the Qt meta object system, such as signal notifications when a list is modified.

For example, the MessageBoard class below has a messages property of type QQmlListProperty that stores a list of Message instances:

class MessageBoard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Message> messages READ messages)
public:
    QQmlListProperty<Message> messages();

private:
    static void append_message(QQmlListProperty<Message> *list, Message *msg);

    QList<Message *> m_messages;
};
The MessageBoard::messages() function simply creates and returns a QQmlListProperty from its QList<T> m_messages member, passing the appropriate list modification functions as required by the QQmlListProperty constructor:

QQmlListProperty<Message> MessageBoard::messages()
{
    return QQmlListProperty<Message>(this, 0, &MessageBoard::append_message);
}

void MessageBoard::append_message(QQmlListProperty<Message> *list, Message *msg)
{
    MessageBoard *msgBoard = qobject_cast<MessageBoard *>(list->object);
    if (msg)
        msgBoard->m_messages.append(msg);
}
Note that the template class type for the QQmlListProperty — in this case, Message — must be registered with the QML type system.

Grouped Properties:
-----------------------------
Any read-only object-type property is accessible from QML code as a grouped property. This can be used to expose a group of related properties that describe a set of attributes for a type.

For example, suppose the Message::author property was of type MessageAuthor rather than a simple string, with sub-properties of name and email:

class MessageAuthor : public QObject
{
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString email READ email WRITE setEmail)
public:
    ...
};

class Message : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MessageAuthor* author READ author)
public:
    Message(QObject *parent)
        : QObject(parent), m_author(new MessageAuthor(this))
    {
    }
    MessageAuthor *author() const {
        return m_author;
    }
private:
    MessageAuthor *m_author;
};
The author property could be written to using the grouped property syntax in QML, like this:

Message {
    author.name: "Alexandra"
    author.email: "alexandra@mail.com"
}
A type that is exposed as a grouped property differs from an object-type property in that the grouped property is read-only, and is initialized to a valid value by the parent object at construction. The grouped property's sub-properties may be modified from QML but the grouped property object itself will never change, whereas an object-type property may be assigned a new object value from QML at any time. Thus, the lifetime of a grouped property object is controlled strictly by the C++ parent implementation, whereas an object-type property can be freely created and destroyed through QML code.

Exposing Methods (Including Qt Slots):
---------------------------------------------

Any method of a QObject-derived type is accessible from QML code if it is:

A public method flagged with the Q_INVOKABLE() macro
A method that is a public Qt slot
For example, the MessageBoard class below has a postMessage() method that has been flagged with the Q_INVOKABLE macro, as well as a refresh() method that is a public slot:

class MessageBoard : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE bool postMessage(const QString &msg) {
        qDebug() << "Called the C++ method with" << msg;
        return true;
    }

public slots:
    void refresh() {
        qDebug() << "Called the C++ slot";
    }
};
If an instance of MessageBoard was set as the context data for a file MyItem.qml, then MyItem.qml could invoke the two methods as shown in the examples below:

C++	
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    MessageBoard msgBoard;
    QQuickView view;
    view.engine()->rootContext()->setContextProperty("msgBoard", &msgBoard);
    view.setSource(QUrl::fromLocalFile("MyItem.qml"));
    view.show();

    return app.exec();
}
QML	
// MyItem.qml
import QtQuick 2.0

Item {
    width: 100; height: 100

    MouseArea {
        anchors.fill: parent
        onClicked: {
            var result = msgBoard.postMessage("Hello from QML")
            console.log("Result of postMessage():", result)
            msgBoard.refresh();
        }
    }
}
If a C++ method has a parameter with a QObject* type, the parameter value can be passed from QML using an object id or a JavaScript var value that references the object.

QML supports the calling of overloaded C++ functions. If there are multiple C++ functions with the same name but different arguments, the correct function will be called according to the number and the types of arguments that are provided.

Values returned from C++ methods are converted to JavaScript values when accessed from JavaScript expressions in QML.

Exposing Signals:
-----------------
Any public signal of a QObject-derived type is accessible from QML code.

The QML engine automatically creates a signal handler for any signal of a QObject-derived type that is used from QML. Signal handlers are always named on<Signal> where <Signal> is the name of the signal, with the first letter capitalized. All parameters passed by the signal are available in the signal handler through the parameter names.

For example, suppose the MessageBoard class has a newMessagePosted() signal with a single parameter, subject:

class MessageBoard : public QObject
{
    Q_OBJECT
public:
   // ...
signals:
   void newMessagePosted(const QString &subject);
};
If the MessageBoard type was registered with the QML type system, then a MessageBoard object declared in QML could receive the newMessagePosted() signal using a signal handler named onNewMessagePosted, and examine the subject parameter value:

MessageBoard {
    onNewMessagePosted: console.log("New message received:", subject)
}
As with property values and method parameters, a signal parameter must have a type that is supported by the QML engine; see Data Type Conversion Between QML and C++. (Using an unregistered type will not generate an error, but the parameter value will not be accessible from the handler.)

Classes may have multiple signals with the same name, but only the final signal is accessible as a QML signal. Note that signals with the same name but different parameters cannot be distinguished from one another.

Data Type Conversion Between QML and C++:(https://doc.qt.io/qt-5/qtqml-cppintegration-data.html)
----------------------------------------
When data values are exchanged between QML and C++, they are converted by the QML engine to have the correct data types as appropriate for use in QML or C++. This requires the exchanged data to be of a type that is recognizable by the engine.

The QML engine provides built-in support for a large number of Qt C++ data types. Additionally, custom C++ types may be registered with the QML type system to make them available to the engine.

This page discusses the data types supported by the QML engine and how they are converted between QML and C++.

Registering C++ Types with the QML Type System
A QObject-derived class can be registered with the QML type system to enable the type to be used as a data type from within QML code.

The engine allows the registration of both instantiable and non-instantiable types. Registering an instantiable type enables a C++ class to be used as the definition of a QML object type, allowing it to be used in object declarations from QML code to create objects of this type. Registration also provides the engine with additional type metadata, enabling the type (and any enums declared by the class) to be used as a data type for property values, method parameters and return values, and signal parameters that are exchanged between QML and C++.

Registering a non-instantiable type also registers the class as a data type in this manner, but the type cannot be used instantiated as a QML object type from QML. This is useful, for example, if a type has enums that should be exposed to QML but the type itself should not be instantiable.

For a quick guide to choosing the correct approach to expose C++ types to QML, see Choosing the Correct Integration Method Between C++ and QML.

Registering an Instantiable Object Type
Any QObject-derived C++ class can be registered as the definition of a QML object type. Once a class is registered with the QML type system, the class can be declared and instantiated like any other object type from QML code. Once created, a class instance can be manipulated from QML; as Exposing Attributes of C++ Types to QML explains, the properties, methods and signals of any QObject-derived class are accessible from QML code.

To register a QObject-derived class as an instantiable QML object type, call qmlRegisterType() to register the class as QML type into a particular type namespace. Clients can then import that namespace in order to use the type.

For example, suppose there is a Message class with author and creationDate properties:

class Message : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    Q_PROPERTY(QDateTime creationDate READ creationDate WRITE setCreationDate NOTIFY creationDateChanged)
public:
    // ...
};
This type can be registered by calling qmlRegisterType() with an appropriate type namespace and version number. For example, to make the type available in the com.mycompany.messaging namespace with version 1.0:

qmlRegisterType<Message>("com.mycompany.messaging", 1, 0, "Message");
The type can be used in an object declaration from QML, and its properties can be read and written to, as per the example below:

import com.mycompany.messaging 1.0

Message {
    author: "Amelie"
    creationDate: new Date()
}
Registering Non-Instantiable Types
Sometimes a QObject-derived class may need to be registered with the QML type system but not as an instantiable type. For example, this is the case if a C++ class:

is an interface type that should not be instantiable
is a base class type that does not need to be exposed to QML
declares some enum that should be accessible from QML, but otherwise should not be instantiable
is a type that should be provided to QML through a singleton instance, and should not be instantiable from QML
The Qt QML module provides several methods for registering non-instantiable types:

qmlRegisterType() (with no parameters) registers a C++ type that is not instantiable and cannot be referred to from QML. This enables the engine to coerce any inherited types that are instantiable from QML.
qmlRegisterInterface() registers an existing Qt interface type. The type is not instantiable from QML, and you cannot declare QML properties with it. Using C++ properties of this type from QML will do the expected interface casts, though.
qmlRegisterUncreatableType() registers a named C++ type that is not instantiable but should be identifiable as a type to the QML type system. This is useful if a type's enums or attached properties should be accessible from QML but the type itself should not be instantiable.
qmlRegisterSingletonType() registers a singleton type that can be imported from QML, as discussed below.
Note that all C++ types registered with the QML type system must be QObject-derived, even if they are non-instantiable.

Registering Singleton Objects with a Singleton Type
A singleton type enables properties, signals and methods to be exposed in a namespace without requiring the client to manually instantiate an object instance. QObject singleton types in particular are an efficient and convenient way to provide functionality or global property values.

Note that singleton types do not have an associated QQmlContext as they are shared across all contexts in an engine. QObject singleton type instances are constructed and owned by the QQmlEngine, and will be destroyed when the engine is destroyed.

A QObject singleton type can be interacted with in a manner similar to any other QObject or instantiated type, except that only one (engine constructed and owned) instance will exist, and it must be referenced by type name rather than id. Q_PROPERTYs of QObject singleton types may be bound to, and Q_INVOKABLE functions of QObject module APIs may be used in signal handler expressions. This makes singleton types an ideal way to implement styling or theming, and they can also be used instead of ".pragma library" script imports to store global state or to provide global functionality.

Once registered, a QObject singleton type may be imported and used like any other QObject instance exposed to QML. The following example assumes that a QObject singleton type was registered into the "MyThemeModule" namespace with version 1.0, where that QObject has a QColor "color" Q_PROPERTY:

import MyThemeModule 1.0 as Theme

Rectangle {
    color: Theme.color // binding.
}
A QJSValue may also be exposed as a singleton type, however clients should be aware that properties of such a singleton type cannot be bound to.

See qmlRegisterSingletonType() for more information on how implement and register a new singleton type, and how to use an existing singleton type.

Note: Enum values for registered types in QML should start with a capital.

QQmlContext:(https://doc.qt.io/qt-5/qqmlcontext.html#details)
---------------
The QQmlContext class defines a context within a QML engine.

Contexts allow data to be exposed to the QML components instantiated by the QML engine.

Each QQmlContext contains a set of properties, distinct from its QObject properties, that allow data to be explicitly bound to a context by name. The context properties are defined and updated by calling QQmlContext::setContextProperty(). The following example shows a Qt model being bound to a context and then accessed from a QML file.

QQmlEngine engine;
QStringListModel modelData;
QQmlContext *context = new QQmlContext(engine.rootContext());
context->setContextProperty("myModel", &modelData);

QQmlComponent component(&engine);
component.setData("import QtQuick 2.0\nListView { model: myModel }", QUrl());
QObject *window = component.create(context);
Note it is the responsibility of the creator to delete any QQmlContext it constructs. If the context object in the example is no longer needed when the window component instance is destroyed, the context must be destroyed explicitly. The simplest way to ensure this is to set window as the parent of context.

To simplify binding and maintaining larger data sets, a context object can be set on a QQmlContext. All the properties of the context object are available by name in the context, as though they were all individually added through calls to QQmlContext::setContextProperty(). Changes to the property's values are detected through the property's notify signal. Setting a context object is both faster and easier than manually adding and maintaining context property values.

The following example has the same effect as the previous one, but it uses a context object.

class MyDataSet : ... {
    ...
    Q_PROPERTY(QAbstractItemModel *myModel READ model NOTIFY modelChanged)
    ...
};

MyDataSet myDataSet;
QQmlEngine engine;
QQmlContext *context = new QQmlContext(engine.rootContext());
context->setContextObject(&myDataSet);

QQmlComponent component(&engine);
component.setData("import QtQuick 2.0\nListView { model: myModel }", QUrl());
component.create(context);
All properties added explicitly by QQmlContext::setContextProperty() take precedence over the context object's properties.

The Context Hierarchy
Contexts form a hierarchy. The root of this hierarchy is the QML engine's root context. Child contexts inherit the context properties of their parents; if a child context sets a context property that already exists in its parent, the new context property overrides that of the parent.

The following example defines two contexts - context1 and context2. The second context overrides the "b" context property inherited from the first with a new value.

QQmlEngine engine;
QQmlContext *context1 = new QQmlContext(engine.rootContext());
QQmlContext *context2 = new QQmlContext(context1);

context1->setContextProperty("a", 12);
context1->setContextProperty("b", 12);

context2->setContextProperty("b", 15);
While QML objects instantiated in a context are not strictly owned by that context, their bindings are. If a context is destroyed, the property bindings of outstanding QML objects will stop evaluating.

Warning: Setting the context object or adding new context properties after an object has been created in that context is an expensive operation (essentially forcing all bindings to reevaluate). Thus whenever possible you should complete "setup" of the context before using it to create any objects.

Q_REVISION:
--------------
As of now this is only used by QtQml, when you need to use a C++ object in QML you must register it using this syntax.

qmlRegisterType<Foo>("MyModule", 1, 0, "Foo");
By doing so you can then import your module in QML and use the Foo object. This syntax exposes to QML all methods and properties marked as Q_REVISION(0), that as the documentation says, is the default value.

Let's say we have our class Foo that has several revisions.

class Foo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int normalProperty READ normalProperty)
    Q_PROPERTY(int newProperty READ newProperty REVISION 1)
    Q_PROPERTY(int newerProperty READ newerProperty REVISION 2)

public:
    Foo();
public slots:
    int normalProperty() { return 0; }
    Q_REVISION(1) int newProperty() { return 1; }
    Q_REVISION(2) int newerProperty() { return 2; }
};
By using the previous syntax we'll be able to only use the normalProperty in QML, to use other revision we must do this.

qmlRegisterType<Foo, 1>("MyModule", 1, 1, "Foo");
Notice the difference in the template arguments, the previous one doesn't have the have revision number after the class name. It's also good practice to increment the minor version of the module, that is the third argument of the function. This way you'll be able to use normalProperty and newProperty in QML.

qmlRegisterType<Foo, 2>("MyModule", 1, 2, "Foo");
With this call instead you'll be able to use all three properties, normalProperty, newProperty and newerProperty.

You can find even more information on the QObject documentation.

Qml property vs alias:
-----------------------
//Component1.qml:

Item
{
    Component2
    {
        contentWidth:200
    }
}

//Component2.qml:

Item
{
    property alias contentWidth:rect.width
    Rectangle
    {
        id:rect
    }
}

There is a very significant difference between your two examples.
A property alias something: id.property automatically creates a bi-directional binding, while in example 1 you only have it one-directional.

Meaning: If you change the value of contentWidth the width of the two Rectangles would change - if now (hypothetically) some inner even in the component Rectangle would change the width, this change would be only reflected in example two, while in example 1 the binding would be broken.

I don't know the implementation details, but it might be easier to optimize it, if you specifically tell, it is not just bound to it, but actually is the same value.

A simple rule of thumbs is: Use property alias ...:... if it is logically an alias, so the properties are intrinsically the same. This is usually the case if you want to export properties from children to the outside, or even whole children.

Use the property binding, if you want to have their value bound because their value coincidencally the same or especially if it is not, but the one is only dependent on the other (but not necessarily other way around, as by default you only have a one-way binding)

A readonly property Item child1: idOfChildOne might have the same effect as property alias child1: idOfChildOne, but semantically the first is like a storage that holds a pointer (that is only forbidden to change, due to the readonly flag) but the second is the direct access to this child. So usually you want to use the second (as done by the QtDesigner).

Property Aliases:
-------------------------
Property aliases are properties which hold a reference to another property. Unlike an ordinary property definition, which allocates a new, unique storage space for the property, a property alias connects the newly declared property (called the aliasing property) as a direct reference to an existing property (the aliased property).

A property alias declaration looks like an ordinary property definition, except that it requires the alias keyword instead of a property type, and the right-hand-side of the property declaration must be a valid alias reference:

[default] property alias <name>: <alias reference>
Unlike an ordinary property, an alias has the following restrictions:

It can only refer to an object, or the property of an object, that is within the scope of the type within which the alias is declared.
It cannot contain arbitrary JavaScript expressions
It cannot refer to objects declared outside of the scope of its type.
The alias reference is not optional, unlike the optional default value for an ordinary property; the alias reference must be provided when the alias is first declared.
It cannot refer to attached properties.
It cannot refer to properties inside a hierarchy with depth 3 or greater. The following code will not work:
property alias color: myItem.myRect.border.color

Item {
    id: myItem
    property Rectangle myRect
}
However, aliases to properties that are up to two levels deep will work.

property alias color: rectangle.border.color

Rectangle {
    id: rectangle
}
For example, below is a Button type with a buttonText aliased property which is connected to the text object of the Text child:

// Button.qml
import QtQuick 2.0

Rectangle {
    property alias buttonText: textItem.text

    width: 100; height: 30; color: "yellow"

    Text { id: textItem }
}
The following code would create a Button with a defined text string for the child Text object:

Button { buttonText: "Click Me" }
Here, modifying buttonText directly modifies the textItem.text value; it does not change some other value that then updates textItem.text. If buttonText was not an alias, changing its value would not actually change the displayed text at all, as property bindings are not bi-directional: the buttonText value would have changed if textItem.text was changed, but not the other way around.

Considerations for Property Aliases
Aliases are only activated once a component has been fully initialized. An error is generated when an uninitialized alias is referenced. Likewise, aliasing an aliasing property will also result in an error.

property alias widgetLabel: label

//will generate an error
//widgetLabel.text: "Initial text"

//will generate an error
//property alias widgetLabelText: widgetLabel.text

Component.onCompleted: widgetLabel.text = "Alias completed Initialization"
When importing a QML object type with a property alias in the root object, however, the property appear as a regular Qt property and consequently can be used in alias references.

It is possible for an aliasing property to have the same name as an existing property, effectively overwriting the existing property. For example, the following QML type has a color alias property, named the same as the built-in Rectangle::color property:

Rectangle {
    id: coloredrectangle
    property alias color: bluerectangle.color
    color: "red"

    Rectangle {
        id: bluerectangle
        color: "#1234ff"
    }

    Component.onCompleted: {
        console.log (coloredrectangle.color)    //prints "#1234ff"
        setInternalColor()
        console.log (coloredrectangle.color)    //prints "#111111"
        coloredrectangle.color = "#884646"
        console.log (coloredrectangle.color)    //prints #884646
    }

    //internal function that has access to internal properties
    function setInternalColor() {
        color = "#111111"
    }
}
Any object that use this type and refer to its color property will be referring to the alias rather than the ordinary Rectangle::color property. Internally, however, the rectangle can correctly set its color property and refer to the actual defined property rather than the alias.

Property Aliases and Types
Property aliases cannot have explicit type specifications. The type of a property alias is the declared type of the property or object it refers to. Therefore, if you create an alias to an object referenced via id with extra properties declared inline, the extra properties won't be accessible through the alias:

// MyItem.qml
Item {
    property alias inner: innerItem

    Item {
        id: innerItem
        property int extraProperty
    }
}
You cannot initialize inner.extraProperty from outside of this component, as inner is only an Item:

// main.qml
MyItem {
    inner.extraProperty: 5 // fails
}
However, if you extract the inner object into a separate component with a dedicated .qml file, you can instantiate that component instead and have all its properties available through the alias:

// MainItem.qml
Item {
    // Now you can access inner.extraProperty, as inner is now an ExtraItem
    property alias inner: innerItem

    ExtraItem {
        id: innerItem
    }
}

// ExtraItem.qml
Item {
    property int extraProperty
}


Default Properties:
---------------------
An object definition can have a single default property. A default property is the property to which a value is assigned if an object is declared within another object's definition without declaring it as a value for a particular property.

Declaring a property with the optional default keyword marks it as the default property. For example, say there is a file MyLabel.qml with a default property someText:

// MyLabel.qml
import QtQuick 2.0

Text {
    default property var someText

    text: "Hello, " + someText.text
}
The someText value could be assigned to in a MyLabel object definition, like this:

MyLabel {
    Text { text: "world!" }
}
This has exactly the same effect as the following:

MyLabel {
    someText: Text { text: "world!" }
}
However, since the someText property has been marked as the default property, it is not necessary to explicitly assign the Text object to this property.

You will notice that child objects can be added to any Item-based type without explicitly adding them to the children property. This is because the default property of Item is its data property, and any items added to this list for an Item are automatically added to its list of children.

Default properties can be useful for reassigning the children of an item. See the TabWidget Example, which uses a default property to automatically reassign children of the TabWidget as children of an inner ListView. See also Extending QML.

Read-Only Properties
An object declaration may define a read-only property using the readonly keyword, with the following syntax:

readonly property <propertyType> <propertyName> : <initialValue>
Read-only properties must be assigned a value on initialization. After a read-only property is initialized, it no longer possible to give it a value, whether from imperative code or otherwise.

For example, the code in the Component.onCompleted block below is invalid:

Item {
    readonly property int someNumber: 10

    Component.onCompleted: someNumber = 20  // doesn't work, causes an error
}
Note: A read-only property cannot also be a default property.
