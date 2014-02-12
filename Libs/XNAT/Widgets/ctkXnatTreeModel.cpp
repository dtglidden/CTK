/*=============================================================================

  Plugin: org.commontk.xnat

  Copyright (c) University College London,
    Centre for Medical Image Computing

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

#include "ctkXnatTreeModel.h"
#include "ctkXnatObject.h"

#include <QList>

ctkXnatTreeModel::ctkXnatTreeModel()
: m_RootItem(new ctkXnatTreeItem())
{
}

ctkXnatTreeModel::~ctkXnatTreeModel()
{
  delete m_RootItem;
}

// returns name (project, subject, etc.) for row and column of
//   parent in index if role is Qt::DisplayRole
QVariant ctkXnatTreeModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
  {
    return QVariant();
  }

  if (role == Qt::TextAlignmentRole)
  {
    return QVariant(int(Qt::AlignTop | Qt::AlignLeft));
  }
  else if (role == Qt::DisplayRole)
  {
    ctkXnatObject* xnatObject = this->xnatObject(index);

    QString displayData = xnatObject->name();
    if (displayData.isEmpty())
    {
      displayData = xnatObject->id();
    }
    return displayData;
  }
  else if (role == Qt::ToolTipRole)
  {
    return this->xnatObject(index)->description();
  }
  else if (role == Qt::UserRole)
  {
    return QVariant::fromValue<ctkXnatObject*>(this->xnatObject(index));
  }

  return QVariant();
}

QModelIndex ctkXnatTreeModel::index(int row, int column, const QModelIndex& index) const
{
  if (!this->hasIndex(row, column, index))
  {
    return QModelIndex();
  }

  ctkXnatTreeItem* item;
  if (!index.isValid())
  {
    item = m_RootItem;
  }
  else
  {
    item = this->itemAt(index);
  }

  ctkXnatTreeItem* childItem = item->child(row);

  if (childItem)
  {
    return this->createIndex(row, column, childItem);
  }

  return QModelIndex();
}

QModelIndex ctkXnatTreeModel::parent(const QModelIndex& index) const
{
  if (!index.isValid())
  {
    return QModelIndex();
  }

  ctkXnatTreeItem* item = this->itemAt(index);
  ctkXnatTreeItem* parentItem = item->parent();

  if (parentItem == m_RootItem)
  {
    return QModelIndex();
  }

  return this->createIndex(parentItem->row(), 0, parentItem);
}

int ctkXnatTreeModel::rowCount(const QModelIndex& index) const
{
  if (index.column() > 0)
  {
    return 0;
  }

  ctkXnatTreeItem* item;
  if (!index.isValid())
  {
    item = m_RootItem;
  }
  else
  {
    item = this->itemAt(index);
  }

  return item->childCount();
}

int ctkXnatTreeModel::columnCount(const QModelIndex& index) const
{
  Q_UNUSED(index);
  return 1;
}

// defer request for children until actually needed by QTreeView object
bool ctkXnatTreeModel::hasChildren(const QModelIndex& index) const
{
  if (!index.isValid())
  {
    return m_RootItem->childCount() > 0;
  }

  ctkXnatTreeItem* item = this->itemAt(index);
  return !item->xnatObject()->isFetched() || !item->xnatObject()->children().isEmpty();
}

bool ctkXnatTreeModel::canFetchMore(const QModelIndex& index) const
{
  if (!index.isValid())
  {
    return false;
  }
  ctkXnatTreeItem* item = this->itemAt(index);
  return !(item->childCount() > 0);
}

void ctkXnatTreeModel::fetchMore(const QModelIndex& index)
{
  if (!index.isValid())
  {
    return;
  }

  ctkXnatTreeItem* item = this->itemAt(index);

  ctkXnatObject* xnatObject = item->xnatObject();

  xnatObject->fetch();

  QList<ctkXnatObject*> children = xnatObject->children();
  if (!children.isEmpty())
  {
    beginInsertRows(index, 0, children.size() - 1);
    foreach (ctkXnatObject* child, children)
    {
      item->appendChild(new ctkXnatTreeItem(child, item));
    }
    endInsertRows();
  }
}

ctkXnatObject* ctkXnatTreeModel::xnatObject(const QModelIndex& index) const
{
  return this->itemAt(index)->xnatObject();
}

void ctkXnatTreeModel::addDataModel(ctkXnatDataModel* dataModel)
{
  m_RootItem->appendChild(new ctkXnatTreeItem(dataModel, m_RootItem));
}

ctkXnatTreeItem* ctkXnatTreeModel::itemAt(const QModelIndex& index) const
{
  return static_cast<ctkXnatTreeItem*>(index.internalPointer());
}


bool ctkXnatTreeModel::removeAllRows(const QModelIndex& parent)
{
  // do nothing for the root
  if ( !parent.isValid() )
  {
    return false;
  }

  ctkXnatObject* xnatObject = this->xnatObject(parent);

  // nt: not sure why the parent.row() is used here instead of the first item in list
  // that is xnatObject->children()[0];
  ctkXnatObject* child = xnatObject->children()[parent.row()];

  if ( child == NULL )
  {
    return false;
  }

  int numberofchildren = child->children().size();
  if (numberofchildren > 0)
  {
    beginRemoveRows(parent, 0, numberofchildren - 1);
    // xnatObject->removeChild(parent.row());
    // nt: not sure if this is the right implementation here, should iterate ?
    xnatObject->remove(child);
    endRemoveRows();
  }
  else
  {
    // xnatObject->removeChild(parent.row());
    // nt: not sure if this is the right implementation here, should iterate ?
    xnatObject->remove(child);
  }
  return true;
}

void ctkXnatTreeModel::downloadFile(const QModelIndex& index, const QString& zipFileName)
{
  if (!index.isValid())
  {
    return;
  }

  this->xnatObject(index)->download(zipFileName);

  return;
}

void ctkXnatTreeModel::uploadFile(const QModelIndex& index, const QString& zipFileName)
{
  if (!index.isValid())
  {
    return;
  }

  ctkXnatObject* xnatObject = this->xnatObject(index);
  ctkXnatObject* child = xnatObject->children()[index.row()];

  child->upload(zipFileName);
}