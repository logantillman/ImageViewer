/*
   Copyright (C) 2017-2020 Peter S. Zhigalov <peter.zhigalov@gmail.com>

   This file is part of the `QtUtils' library.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#if !defined (QTUTILS_LOCALIZATION_MANAGER_H_INCLUDED)
#define QTUTILS_LOCALIZATION_MANAGER_H_INCLUDED

#include <QObject>
#include <QStringList>
#include "ScopedPointer.h"

class QMenu;
class QComboBox;

namespace Locale {

extern const QString EN;
extern const QString RU;

} // namespace Locale

/// @brief Менеджер по управлению локализациями.
class LocalizationManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(LocalizationManager)

Q_SIGNALS:
    /// @brief Сигнал об изменении локализации.
    /// @param locale - Новая локаль.
    void localeChanged(const QString &locale);

public:
    /// @brief Получить указатель на экземпляр текущего менеджера локализаций.
    /// @return Указатель на экземпляр текущего менеджера локализаций.
    static LocalizationManager *instance();

    /// @brief Инициализировать трансляторы для указанных ресурсов.
    /// @param templatesList - пути к локализационным ресурсным файлам,
    ///  например ":/translations/qtutils_%1" (%1 - шаблон для локали).
    void initializeResources(const QStringList &templatesList);

    /// @brief Получить текущую локаль.
    /// @return Текущая локаль.
    QString locale() const;

    /// @brief Установить новую локаль.
    /// @param locale - Новая локаль.
    void setLocale(const QString &locale = QString());

    /// @brief Заполнить меню элементами для выбора локали. Все необходимые
    ///  соединения будут установлены автоматически.
    /// @param menu - меню, которое должно быть заполнено.
    void fillMenu(QMenu *menu);

    /// @brief Заполнить комбобокс элементами для выбора локали. Все необходимые
    ///  соединения будут установлены автоматически.
    /// @param comboBox - комбобокс, который должен быть заполнен.
    /// @param autoApply - автоматически применять изменения при выборе локали
    void fillComboBox(QComboBox *comboBox, const bool autoApply = true);

private:
    LocalizationManager();
    ~LocalizationManager();

private Q_SLOTS:
    void onActionEnglishTriggered();
    void onActionRussianTriggered();
    void onComboBoxActivated(int index);
    void onActionDestroyed(QObject *object);
    void onComboBoxDestroyed(QObject *object);

private:
    struct Impl;
    QScopedPointer<Impl> m_impl;
};

#endif // QTUTILS_LOCALIZATION_MANAGER_H_INCLUDED

