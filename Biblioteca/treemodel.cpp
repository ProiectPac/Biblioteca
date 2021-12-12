/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*
    treemodel.cpp

    Provides a simple tree model to show how to create and use hierarchical
    models.
*/

#include "treemodel.h"
#include "treeitem.h"

#include <QStringList>

//! [0]
TreeModel::TreeModel(std::vector<Book> bookVector, bool type,QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new TreeItem({tr("Book Title")});
    this->type = type;
    setUpModel(bookVector);
}
//! [0]

//! [1]
TreeModel::~TreeModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

void TreeModel::setUpModel(std::vector<Book> bookVector)
{
    for(auto& book : bookVector)
    {
        QList<QVariant> titleData;
        titleData << book.getTitle();
        TreeItem* child = new TreeItem(titleData,rootItem);

        QList<QVariant> idData;
        idData << "ID: " + QString::number(book.getID());
        TreeItem* id = new TreeItem(idData,child);

        QList<QVariant> isbnData;
        isbnData << "ISBN: " + book.getISBN();
        TreeItem* isbn = new TreeItem(isbnData,child);

        QList<QVariant> authorsData;
        authorsData << "Authors: " + book.getAuthor();
        TreeItem* authors = new TreeItem(authorsData,child);

        QList<QVariant> originalPublicationYearData;
        originalPublicationYearData << "Publication year: " + QString::number(book.getOriginalPublicationYear());
        TreeItem* originalPublicationYear = new TreeItem(originalPublicationYearData,child);

        QList<QVariant> languageCodeData;
        languageCodeData << "Language: " + book.getLanguage();
        TreeItem* languageCode = new TreeItem(languageCodeData,child);

        QList<QVariant> averageRatingData;
        averageRatingData << "Average rating: " + QString::number(book.getAverageRating());
        TreeItem* averageRating = new TreeItem(averageRatingData,child);

        child->appendChild(id);
        child->appendChild(isbn);
        child->appendChild(authors);
        child->appendChild(originalPublicationYear);
        child->appendChild(languageCode);
        child->appendChild(averageRating);

        if(type==false)
        {
            QList<QVariant> booksCountData;
            booksCountData << "Books Count: " + QString::number(book.getBooksCount());
            TreeItem* booksCount = new TreeItem(booksCountData,child);
            child->appendChild(booksCount);
        }

        if(type == true)
        {
            QList<QVariant> remainingDaysData;
            remainingDaysData << "Remaining days: " + QString::number(book.getRemainingDays());
            TreeItem* remainingDays = new TreeItem(remainingDaysData,child);
            child->appendChild(remainingDays);
        }

        rootItem->appendChild(child);
    }
}
//! [2]

//! [3]
QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}
//! [3]

//! [4]
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}
//! [4]

//! [5]
QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}
//! [5]

//! [6]
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}
//! [6]

//! [7]
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
//! [7]

//! [8]
int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
//! [8]

