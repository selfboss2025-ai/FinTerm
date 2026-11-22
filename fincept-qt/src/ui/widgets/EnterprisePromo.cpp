#include "ui/widgets/EnterprisePromo.h"

#include <QLabel>
#include <QPushButton>

namespace fincept::ui {

namespace enterprise {

QString site_url() {
    return {};
}
QString pricing_url() {
    return {};
}
QString comparison_url() {
    return {};
}
QString signup_url() {
    return {};
}
QString meeting_url() {
    return {};
}
QString partnership_mailto() {
    return {};
}
QString url_with_ref(const QString&, const QString&) {
    return {};
}
void open_url(const QString&) {}

QPushButton* make_cta_button(const QString&, const QString&, CtaStyle, QWidget* parent) {
    auto* btn = new QPushButton(parent);
    btn->setVisible(false);
    btn->setFixedSize(0, 0);
    return btn;
}

void set_cta_style(QPushButton*, CtaStyle) {}

QLabel* make_mention_label(QWidget* parent) {
    auto* label = new QLabel(parent);
    label->setVisible(false);
    label->setFixedSize(0, 0);
    return label;
}

void set_mention_text(QLabel* label, const QString&, const QString&, const QString&) {
    if (label) {
        label->setVisible(false);
        label->setFixedSize(0, 0);
    }
}

QPushButton* make_partnership_link(QWidget* parent) {
    auto* btn = new QPushButton(parent);
    btn->setVisible(false);
    btn->setFixedSize(0, 0);
    return btn;
}

void set_fitting_text(QPushButton*, const QString&, const QString&) {}

bool session_has_enterprise_plan() {
    return true;
}

bool placement_dismissed(const QString&) {
    return true;
}

void dismiss_placement(const QString&) {}

} // namespace enterprise

namespace billing {

void open_plans(QWidget*) {}

} // namespace billing

// ── UpgradeDialog (disabled — FinTerminal is completely free) ────────────────

UpgradeDialog::UpgradeDialog(QWidget* parent, const QString& placement) : QDialog(parent), placement_(placement) {
    setVisible(false);
}

void UpgradeDialog::changeEvent(QEvent* event) {
    QDialog::changeEvent(event);
}

void UpgradeDialog::retranslateUi() {}
void UpgradeDialog::refresh_theme() {}

void UpgradeDialog::show_now(QWidget*, const QString&) {}
void UpgradeDialog::maybe_show(QWidget*) {}

// ── EnterpriseShowcase (disabled) ────────────────────────────────────────────

EnterpriseShowcase::EnterpriseShowcase(QWidget* parent) : QWidget(parent) {
    setVisible(false);
    setFixedSize(0, 0);
}

void EnterpriseShowcase::set_placement(const QString& placement) {
    placement_ = placement;
}

QVector<EnterpriseShowcase::Slide> EnterpriseShowcase::slides() const {
    return {};
}

void EnterpriseShowcase::show_slide(int, bool) {}
void EnterpriseShowcase::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    setVisible(false);
}
void EnterpriseShowcase::hideEvent(QHideEvent* event) {
    QWidget::hideEvent(event);
}
void EnterpriseShowcase::changeEvent(QEvent* event) {
    QWidget::changeEvent(event);
}
void EnterpriseShowcase::retranslateUi() {}
void EnterpriseShowcase::refresh_theme() {}

QWidget* make_enterprise_showcase(QWidget* parent) {
    return new EnterpriseShowcase(parent);
}

// ── EnterpriseLockPromo (disabled) ───────────────────────────────────────────

EnterpriseLockPromo::EnterpriseLockPromo(Form form, QWidget* parent) : QWidget(parent), form_(form) {
    setVisible(false);
    setFixedSize(0, 0);
}

void EnterpriseLockPromo::build_panel() {}
void EnterpriseLockPromo::build_strip() {}
int EnterpriseLockPromo::panel_min_width() {
    return 0;
}
int EnterpriseLockPromo::panel_max_width() {
    return 0;
}
QStringList EnterpriseLockPromo::panel_points() const {
    return {};
}
QStringList EnterpriseLockPromo::strip_points() const {
    return {};
}
void EnterpriseLockPromo::retranslateUi() {}
void EnterpriseLockPromo::fit_strip() {}
void EnterpriseLockPromo::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    setVisible(false);
}
void EnterpriseLockPromo::hideEvent(QHideEvent* event) {
    QWidget::hideEvent(event);
}
void EnterpriseLockPromo::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
}
void EnterpriseLockPromo::changeEvent(QEvent* event) {
    QWidget::changeEvent(event);
}

} // namespace fincept::ui
