#include "StudioController.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QFrame>
#include <QStyle>

StudioController::StudioController(QObject *parent) : QObject(parent) {
  m_undoStack = new QUndoStack(this);
}

void StudioController::manageWidget(QWidget *widget) {
  QElapsedTimer timer;
  timer.start();

  // Instalamos o filtro para interceptar cliques
  widget->installEventFilter(this);

  // Recursivamente para filhos (ex: botões dentro de frames)
  for (QObject *child : widget->children()) {
    if (QWidget *w = qobject_cast<QWidget *>(child)) {
      manageWidget(w);
    }
  }

  // Log apenas se demorar mais que 1ms para evitar spam em recursão
  if (timer.elapsed() > 1) {
    qDebug() << "[PERF] manageWidget" << widget << "took" << timer.elapsed()
             << "ms";
  }
}

bool StudioController::eventFilter(QObject *watched, QEvent *event) {
  if (event->type() == QEvent::MouseButtonPress) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    QWidget *widget = qobject_cast<QWidget *>(watched);
    if (widget) {
      // Se for clique com botão direito em um item já selecionado,
      // não alteramos a seleção para permitir que o menu de contexto
      // atue sobre o grupo atual.
      if (mouseEvent->button() == Qt::RightButton &&
          m_selectedWidgets.contains(widget)) {
        return false; // Deixa o evento propagar para disparar o ContextMenu
      }

      if (mouseEvent->modifiers() & Qt::ControlModifier) {
        multiSelectWidget(widget);
      } else {
        selectWidget(widget);
      }
      return true;
    }
  }
  return QObject::eventFilter(watched, event);
}

void StudioController::selectWidget(QWidget *widget) {
  QElapsedTimer timer;
  timer.start();

  // Se clicar no mesmo widget único já selecionado, não fazer nada (otimização)
  if (m_selectedWidgets.size() == 1 && m_selectedWidgets.first() == widget) {
    return;
  }

  // Limpar destaques antigos
  for (QWidget *w : m_selectedWidgets) {
    if (w) {
      // Só desmarcar se realmente estava marcado
      if (w->property("selected").toBool()) {
        w->setProperty("selected", false);
        w->style()->unpolish(w);
        w->style()->polish(w);
      }
    }
  }
  m_selectedWidgets.clear();

  if (widget) {
    m_selectedWidgets.append(widget);
    // Só marcar se não estava
    if (!widget->property("selected").toBool()) {
      widget->setProperty("selected", true);
      widget->style()->unpolish(widget);
      widget->style()->polish(widget);
    }
  }

  emit selectionChanged();
  emit widgetSelected(widget);

  // Log apenas se demorar
  if (timer.elapsed() > 5) {
    qDebug() << "[PERF] selectWidget took" << timer.elapsed() << "ms";
  }
}

void StudioController::multiSelectWidget(QWidget *widget) {
  if (!widget)
    return;

  if (m_selectedWidgets.contains(widget)) {
    widget->setProperty("selected", false);
    m_selectedWidgets.removeOne(widget);
  } else {
    m_selectedWidgets.append(widget);
    widget->setProperty("selected", true);
  }

  widget->style()->unpolish(widget);
  widget->style()->polish(widget);

  emit selectionChanged();
  emit widgetSelected(m_selectedWidgets.isEmpty() ? nullptr
                                                  : m_selectedWidgets.last());
}
