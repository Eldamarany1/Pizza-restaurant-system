// mainwindow.cpp
#include "mainwindow.h"       // include our header
#include "ui_mainwindow.h"    // include the UI generated from .ui file
#include <QMessageBox>        // for popup dialogs

// ——— Pizza implementation ———

Pizza::Pizza(const QString &name, double price)
    : m_name(name)     // initialize m_name with name
    , m_price(price)   // initialize m_price with price
{}

QString Pizza::name() const {  // return the pizza’s name
    return m_name;
}

double Pizza::price() const {  // return the pizza’s price
    return m_price;
}

// ——— OrderItem implementation ———

OrderItem::OrderItem(const Pizza &pizza, int qty)
    : m_pizza(pizza)   // copy the Pizza object
    , m_qty(qty)       // set quantity
{}

Pizza OrderItem::pizza() const {    // getter for m_pizza
    return m_pizza;
}

int OrderItem::quantity() const {   // getter for m_qty
    return m_qty;
}

double OrderItem::totalPrice() const { // compute line total
    return m_pizza.price() * m_qty;
}

// ——— Order implementation ———

void Order::addItem(const OrderItem &item) {
    m_items.push_back(item);      // append item to vector
}

double Order::total() const {
    double sum = 0;               // accumulator
    for (auto &it : m_items)      // iterate all items
        sum += it.totalPrice();   // add each line’s total
    return sum;                   // return grand total
}

const std::vector<OrderItem>& Order::items() const {
    return m_items;               // give read‐only access
}

void Order::clear() {
    m_items.clear();              // remove all items
}

// ——— Payment implementation ———

Payment::Payment(const Order &order, PaymentMethod method)
    : m_order(order)               // copy the order
    , m_method(method)             // save method
    , m_amount(order.total())      // calculate amount
{}

bool Payment::process() {
    // In a real system we'd call an API based on m_method
    // Here we simulate success unconditionally
    return true;
}

double Payment::amount() const {
    return m_amount;              // return what we’ll charge
}

PaymentMethod Payment::method() const {
    return m_method;              // return chosen method
}

// ——— MainWindow implementation ———

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)         // initialize base
    , ui(new Ui::MainWindow)      // allocate UI instance
{
    ui->setupUi(this);           // hook up widgets from .ui file

    // 1) Build the in‐memory pizza menu
    m_menu = {
        Pizza("Margherita", 6.00),
        Pizza("Pepperoni", 8.00),
        Pizza("Hawaiian", 9.00)
    };

    // 2) Populate combo box with pizza names
    for (auto &pz : m_menu)
        ui->pizzaComboBox->addItem(pz.name());
}

MainWindow::~MainWindow() {
    delete ui;                    // free UI instance
}

void MainWindow::on_addToOrderButton_clicked() {
    int idx = ui->pizzaComboBox->currentIndex(); // which pizza
    int qty = ui->quantitySpinBox->value();      // how many
    if (idx < 0 || qty < 1) return;              // guard: invalid

    // create a new OrderItem and add it
    OrderItem item(m_menu[idx], qty);
    m_currentOrder.addItem(item);

    // refresh the order list widget
    ui->orderListWidget->clear();                // clear old entries
    for (auto &it : m_currentOrder.items()) {
        // append a line like "Pepperoni x2 = $16.00"
        ui->orderListWidget->addItem(
            QString("%1 x%2 = $%3")
                .arg(it.pizza().name())
                .arg(it.quantity())
                .arg(it.totalPrice(), 0, 'f', 2)
            );
    }

    // update the total label at bottom
    ui->totalLabel->setText(
        QString("$%1").arg(m_currentOrder.total(), 0, 'f', 2)
        );
}

void MainWindow::on_payNowButton_clicked() {
    // read the chosen payment text
    QString pm = ui->paymentComboBox->currentText();
    PaymentMethod method = PaymentMethod::Cash; // default

    // map text → enum
    if (pm == "Credit Card")       method = PaymentMethod::CreditCard;
    else if (pm == "Mobile Pay")   method = PaymentMethod::MobilePay;

    // create & process payment
    Payment pay(m_currentOrder, method);
    if (pay.process()) {
        // success dialog
        QMessageBox::information(
            this,
            "Payment",
            QString("Paid %1 successfully via %2")
                .arg(pay.amount(), 0, 'f', 2)
                .arg(pm)
            );
        // clear everything for next order
        m_currentOrder.clear();
        ui->orderListWidget->clear();
        ui->totalLabel->setText("$0.00");
    } else {
        // failure dialog (not used here)
        QMessageBox::warning(this, "Payment Failed", "Please try again.");
    }
}
