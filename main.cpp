#include <iostream>
#include <memory>
#include <vector>
#include <string>

struct MessageEvent {
    std::string message;
};

class Observer {
public:
    virtual ~Observer() = default;
    virtual void OnEvent(const MessageEvent& message_event) = 0;
};

class Subject {
public:
    virtual ~Subject() = default;
    virtual void Attach(const std::shared_ptr<Observer>& observer) = 0;
    virtual void Detach(const std::shared_ptr<Observer>& observer) = 0;
    virtual void Notify(const std::string& message) = 0;
};

class ConcreteSubject : public Subject {
    std::vector<std::shared_ptr<Observer>> listObserver;
    std::string subjectName;
public:
    explicit ConcreteSubject(const std::string& subjectName) : subjectName(subjectName) {
        std::cout << "ConcreteSubject " << this->subjectName << " created" << std::endl;
    }

    ~ConcreteSubject() override {
        std::cout << "ConcreteSubject " << this->subjectName << " deleted" << std::endl;
    }

    void Attach(const std::shared_ptr<Observer>& observer) override {
        listObserver.push_back(observer);
    }

    void Detach(const std::shared_ptr<Observer>& observer) override {
        listObserver.erase(std::remove(listObserver.begin(), listObserver.end(), observer), listObserver.end());
    }

    void Notify(const std::string& message) override {
        for (const auto& observer : listObserver) {
            observer->OnEvent(MessageEvent{message + " from " + subjectName});
        }
    }
};

class ConcreteObserver : public Observer {
    std::string observerName;
public:
    explicit ConcreteObserver(const std::string& observerName) : observerName(observerName) {
        std::cout << "ConcreteObserver " << this->observerName << " created" << std::endl;
    }

    ~ConcreteObserver() override {
        std::cout << "ConcreteObserver " << this->observerName << " deleted" << std::endl;
    }

    void OnEvent(const MessageEvent& message_event) override {
        std::cout << "Message: " << message_event.message << " on observer " << observerName <<std::endl;
    }
};


int main() {
    auto subject1 = std::make_unique<ConcreteSubject>("Subject1");
    auto subject2 = std::make_unique<ConcreteSubject>("Subject2");
    auto observer1 = std::make_shared<ConcreteObserver>("Observer1");
    auto observer2 = std::make_shared<ConcreteObserver>("Observer2");
    auto observer3 = std::make_shared<ConcreteObserver>("Observer3");

    {
        auto subject3 = std::make_unique<ConcreteSubject>("Subject3");
        auto observer4 = std::make_shared<ConcreteObserver>("Observer4");
        subject3->Attach(observer4);
        subject3->Attach(observer3);
        subject3->Notify("Message1");
    }

    subject1->Attach(observer1);
    subject1->Attach(observer2);
    subject2->Attach(observer2);
    subject2->Attach(observer3);


    subject1->Notify("Message2");
    subject2->Notify("Message3");

    subject1->Detach(observer1);

    subject1->Notify("Message4");

    subject1->Detach(observer2);

    subject1->Notify("Message3");
    return 0;
}