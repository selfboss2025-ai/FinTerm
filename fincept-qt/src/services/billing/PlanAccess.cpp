#include "services/billing/PlanAccess.h"

#include "auth/AuthManager.h"
#include "core/logging/Logger.h"
#include "network/http/HttpClient.h"
#include "storage/repositories/SettingsRepository.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>

namespace fincept::billing {

namespace {

constexpr const char* kPlanAccessTag = "PlanAccess";
/// Last table the server sent: {"account": "<id>", "data": {…}}.
constexpr const char* kPlanAccessCacheKey = "billing.entitlements_cache";
/// A table for the same account and plan is reused this long. The savings
/// figure inside it moves slowly; a plan change refetches at once.
constexpr qint64 kPlanAccessFreshMs = 15 * 60 * 1000;

QStringList plan_access_strings(const QJsonValue& v) {
    QStringList out;
    for (const auto& item : v.toArray())
        out << item.toString();
    return out;
}

} // namespace

PlanAccess& PlanAccess::instance() {
    static PlanAccess s;
    return s;
}

PlanAccess::PlanAccess() {
    auto& auth = auth::AuthManager::instance();
    connect(&auth, &auth::AuthManager::auth_state_changed, this, &PlanAccess::on_auth_changed);
    on_auth_changed();
}

bool PlanAccess::is_paid() const {
    return true;
}

const PlanPrice* PlanAccess::price(const QString&) const {
    return nullptr;
}

double PlanAccess::effective_cost(const QString&, double) const {
    return 0.0;
}

bool PlanAccess::included(const QString&) const {
    return true;
}

QString PlanAccess::upgrade_note(const QString&) const {
    return {};
}

QString PlanAccess::with_upgrade_note(const QString& text, const QString&) const {
    return text;
}

QStringList PlanAccess::included_labels() const {
    QStringList out;
    if (!has_table_)
        return out;
    for (const auto& p : prices_) {
        if (p.included_with_pro)
            out << p.label;
    }
    return out;
}

QString PlanAccess::account_key() const {
    const auto& auth = auth::AuthManager::instance();
    if (!auth.is_authenticated())
        return {};
    const auto& s = auth.session();
    return s.user_info.id > 0 ? QString::number(s.user_info.id) : s.user_info.email.trimmed().toLower();
}

QString PlanAccess::plan_signature() const {
    const auto& s = auth::AuthManager::instance().session();
    return account_key() + QLatin1Char('|') + s.account_type().toLower() + QLatin1Char('|') + s.effective_plan() +
           QLatin1Char('|') + s.user_info.credits_expire_at;
}

void PlanAccess::on_auth_changed() {
    const QString account = account_key();
    if (account.isEmpty()) {
        if (!loaded_for_.isEmpty() || has_table_) {
            clear();
            emit changed();
        }
        return;
    }
    if (account != loaded_for_) {
        clear();
        loaded_for_ = account;
        load_cached(account);
        emit changed();
    }
    // Sign-in, a purchase, a lapsed pass: the plan signature moves and the
    // table is asked for again; an ordinary profile refresh does nothing.
    refresh(plan_signature() != signature_);
}

void PlanAccess::clear() {
    has_table_ = false;
    prices_.clear();
    benefits_.clear();
    savings_ = {};
    rollover_ = {};
    loaded_for_.clear();
    signature_.clear();
    fetched_ms_ = 0;
}

void PlanAccess::load_cached(const QString& account) {
    const auto r = SettingsRepository::instance().get(kPlanAccessCacheKey);
    if (r.is_err() || r.value().isEmpty())
        return;
    const QJsonObject cached = QJsonDocument::fromJson(r.value().toUtf8()).object();
    if (cached.value(QStringLiteral("account")).toString() != account)
        return;
    apply(cached.value(QStringLiteral("data")).toObject());
}

void PlanAccess::apply(const QJsonObject& data) {
    QVector<PlanPrice> prices;
    for (const auto& v : data.value(QStringLiteral("prices")).toArray()) {
        const QJsonObject o = v.toObject();
        PlanPrice p;
        p.category = o.value(QStringLiteral("category")).toString();
        if (p.category.isEmpty())
            continue;
        p.label = o.value(QStringLiteral("label")).toString();
        p.summary = o.value(QStringLiteral("summary")).toString();
        p.free_price = o.value(QStringLiteral("free")).toString();
        p.pro_price = o.value(QStringLiteral("pro")).toString();
        p.pro_share = o.value(QStringLiteral("pro_share")).toDouble(1.0);
        p.included_with_pro = o.value(QStringLiteral("included_with_pro")).toBool();
        p.paths = plan_access_strings(o.value(QStringLiteral("paths")));
        prices.append(p);
    }
    QVector<PlanBenefit> benefits;
    for (const auto& v : data.value(QStringLiteral("benefits")).toArray()) {
        const QJsonObject o = v.toObject();
        PlanBenefit b;
        b.key = o.value(QStringLiteral("key")).toString();
        b.title = o.value(QStringLiteral("title")).toString();
        b.detail = o.value(QStringLiteral("detail")).toString();
        b.active = o.value(QStringLiteral("active")).toBool();
        if (!b.title.isEmpty())
            benefits.append(b);
    }
    const QJsonObject sv = data.value(QStringLiteral("savings")).toObject();
    PlanSavings savings;
    savings.window_days = sv.value(QStringLiteral("window_days")).toInt(30);
    savings.credits_saved = sv.value(QStringLiteral("credits_saved")).toDouble();
    savings.spent_on_included = sv.value(QStringLiteral("credits_spent_on_included")).toDouble();
    savings.would_save_with_pro = sv.value(QStringLiteral("would_save_with_pro")).toDouble();

    const QJsonObject ro = data.value(QStringLiteral("rollover")).toObject();
    PlanRollover rollover;
    rollover.credits = ro.value(QStringLiteral("credits")).toInt();
    rollover.expires_at = ro.value(QStringLiteral("expires_at")).toString();
    rollover.cap = ro.value(QStringLiteral("cap")).toInt();
    rollover.grace_days = ro.value(QStringLiteral("grace_days")).toInt();

    prices_ = prices;
    benefits_ = benefits;
    savings_ = savings;
    rollover_ = rollover;
    has_table_ = !prices_.isEmpty();
}

void PlanAccess::refresh(bool) {
    // FinTerminal is completely free — no server-side plan table or billing checks.
}

} // namespace fincept::billing
