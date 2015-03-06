.pragma library

/* RedBlackTree code adapted from Julienne Walker:
 * http://www.eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx
 */
function RedBlackTree()
{
    this.Node = function(key, value, userCount) {
        this.key = key;
        this.value = value;
        this.parent = null;
        this.children = [null, null];
        this.color = 1; // initialize with red
        userCount = userCount !== undefined ? userCount : 0;
        this.userCount = userCount;
        this.counts = [1, userCount];
    }
    this.Node.prototype.dump = function(indent) {
        var str = indent
            + this.key + ": "
            + this.value + " "
            + (this.color == 0 ? "b" : "r")
            + " " + this.counts[0]
            + " user: " + this.userCount
            + " user total: " + this.counts[1]
            + "\n";
        if ((this.children[0] !== null) || (this.children[1] !== null))
        {
            for(var c = 0; c < 2; c++)
            {
                if (this.children[c] !== null)
                    str += this.children[c].dump(indent + "\t");
                else
                    str += indent + "\t(empty)\n";
            }
        }
        return str;
    }

    this.root = null;
}

RedBlackTree.prototype.check = function() {
    this.checkString = "";
    this._check(this.root);
    return this.checkString;
}

RedBlackTree.prototype._check = function(node) {
    if (node === null)
        return 0;

    var left = node.children[0];
    var right = node.children[1];
    // Consecutive red nodes
    if (this._isRed (node))
    {
        if (this._isRed (left) || this._isRed(right))
        {
            this.checkString = "Red violation at " + node.key;
            return 0;
        }
    }

    var lh = this._check(left);
    var rh = this._check(right);

    /* Invalid binary search tree */
    if (((left !== null) && (left.key > node.key))
        || ((right !== null) && (right.key < node.key)))
    {
        this.checkString = "Binary tree violation at " + node.key;
        return 0;
    }

    if (this.checkString != "")
        return 0;

    /* Black height mismatch */
    if ( (lh != 0) && (rh != 0) && (lh != rh) ) {
        this.checkString = "Black violation at " + node.key;
        return 0;
    }

    /* Only count black links */
    if ((lh != 0) && (rh != 0))
      return this._isRed (node) ? lh : lh + 1;
    else
      return 0;
}

RedBlackTree.prototype.dump = function() {
    if (this.root === null)
        return "(empty)";
    else
        return this.root.dump("");
}

RedBlackTree.prototype._updateCount = function(node) {
    for (var n = 0; n < 2; n++)
    {
        var c = ((node.children[0] !== null) ? node.children[0].counts[n] : 0)
              + ((node.children[1] !== null) ? node.children[1].counts[n] : 0)
              + ((n === 0) ? 1 : node.userCount);
        node.counts[n] = c;
    }
}

RedBlackTree.prototype._internalInsert = function(node, newNode) {
    if (node === null)
    {
        return newNode;
    }
    else
    {
        var d = (node.key < newNode.key) ? 1 : 0;
        node.children[d] = this._internalInsert(node.children[d], newNode);

        // Rebalance
        if ( this._isRed (node.children[d]) )
        {
            if ( this._isRed (node.children[d^1]) )
            {
                /* Case 1 (two red children, make parent black) */
                node.color = 1;
                node.children[0].color = 0;
                node.children[1].color = 0;
            }
            else {
                /* Cases 2 & 3 (children of mixed colors) */
                if ( this._isRed (node.children[d].children[d]) )
                    node = this._rotate (node, d^1);
                else if (this._isRed (node.children[d].children[d^1]) )
                    node = this._rotate2 (node, d^1);
            }
        }

        this._updateCount(node);

        return node;
    }
}

RedBlackTree.prototype._isRed = function(node) {
    return (node !== null) && (node.color === 1);
}

RedBlackTree.prototype._isBlack = function(node) {
    return (node === null) || (node.color === 0);
}

RedBlackTree.prototype.insert = function(key, value) {
    var n = new this.Node(key, value);
    this.root = this._internalInsert (this.root, n);
    this.root.color = 0;
}

// Single rotation
RedBlackTree.prototype._rotate = function(node, d) {
    var save = node.children[d^1];
    node.children[d^1] = save.children[d];
    save.children[d] = node;

    node.color = 1;
    save.color = 0;

    this._updateCount(node);
    this._updateCount(save);

    return save;
}

// Double rotation
RedBlackTree.prototype._rotate2 = function(node, d) {
    node.children[d^1] = this._rotate(node.children[d^1], d^1);
    return this._rotate(node, d);
}

RedBlackTree.prototype.remove = function(key) {
    if ( this.root !== null )
    {
        /* False tree root */
        var head = new this.Node(undefined);
        /* Helpers */
        var q, p, g;
        /* Found item */
        var f;
        var dir = 1;

        /* Set up helpers */
        q = head;
        g = null;
        p = null;
        q.children[1] = this.root;

        /* Search and push a red down */
        while (q.children[dir] !== null)
        {
            var last = dir;

            /* Update helpers */
            g = p;
            p = q;
            q = q.children[dir];
            dir = (q.key < key) ? 1 : 0;

            /* Save found node */
            if ( q.key === key )
                f = q;

            /* Push the red node down */
            if (this._isBlack (q) && this._isBlack (q.children[dir]))
            {
                if (this._isRed(q.children[dir^1]))
                {
                    p = p.children[last] = this._rotate (q, dir);
                }
                else if (this._isBlack (q.children[dir^1]))
                {
                    var s = p.children[last^1];

                    if ( s !== null )
                    {
                        if ( this._isBlack (s.children[last^1] ) && this._isRed(s.children[last]))
                        {
                            /* Color flip */
                            p.color = 0;
                            s.color = 1;
                            q.color = 1;
                        }
                        else
                        {
                            var dir2 = g.children[1] === p ? 1 : 0;

                            if (this._isRed(s.children[last]))
                            {
                                g.children[dir2] = this._rotate2 (p, last);
                            }
                            else if (this._isRed (s.children[last^1]))
                            {
                                g.children[dir2] = this._rotate (p, last);
                            }

                            /* Ensure correct coloring */
                            q.color = g.children[dir2].color = 1;
                            g.children[dir2].children[0].color = 0;
                            g.children[dir2].children[1].color = 0;
                        }
                    }
                }
            }
        }

        /* Replace and remove if found */
        if (f !== undefined)
        {
            f.key = q.key;
            f.value = q.value;
            p.children[p.children[1] === q ? 1 : 0] = q.children[q.children[0] === null ? 1 : 0];
            this._updateCountAfterRemove (head.children[1], p.key);
        }

        /* Update root and make it black */
        this.root = head.children[1];
        if ( this.root !== null )
            this.root.color = 0;
    }
}

RedBlackTree.prototype._updateCountAfterRemove = function(node, key) {
    if (node === null) return;
    var dir = (node.key < key) ? 1 : 0;
    this._updateCountAfterRemove(node.children[dir], key);
    this._updateCount(node);
}

RedBlackTree.prototype._queryCount = function (key, n) {
    if (this.root === null) return undefined;
    /* The "count" is sum of all node counts up until the node itself
       if the tree was traversed depth-first left-to-right.
       Determine this number w/o doing a whole traversal. */
    var node = this.root;
    var index = 0;
    while (node !== null)
    {
        if (node.key === key)
        {
            // Node found
            // Increase current index by the count sum of the left subtree.
            if (node.children[0] !== null)
                index += node.children[0].counts[n];
            node = null;
        }
        else if (node.key < key)
        {
            // Go right.
            // The index must be increased by the count sum of the left subtree.
            if (node.children[0] !== null)
                index += node.children[0].counts[n];
            // And, of course, the node itself
            index += (n === 0) ? 1 : node.userCount;
            node = node.children[1];
        }
        else
        {
            // Go left
            // Index can stay as-is
            node = node.children[0];
        }
    }
    return index;
}

RedBlackTree.prototype.querySortedIndex = function (key) {
    /* The "sorted index" is the position a node would have
       if the tree was traversed depth-first left-to-right.
       Determine this number w/o doing a whole traversal. */
    return this._queryCount(key, 0);
}

RedBlackTree.prototype.queryUserCount = function (key) {
    return this._queryCount(key, 1);
}

RedBlackTree.prototype.queryValue = function (key) {
    var node = this.root;
    while (node !== null)
    {
        if (node.key === key)
        {
            return node.value;
        }
        else
        {
            var dir = (node.key < key) ? 1 : 0;
            node = node.children[dir];
        }
    }
    return undefined;
}

RedBlackTree.prototype.count = function() {
    return this.root !== null ? this.root.counts[0] : 0;
}

RedBlackTree.prototype.totalUserCount = function() {
    return this.root !== null ? this.root.counts[1] : 0;
}

RedBlackTree.prototype._setUserCount = function(node, key, count) {
    if (node === null) return;
    if (node.key === key)
    {
        node.userCount = count;
    }
    else
    {
        var dir = (node.key < key) ? 1 : 0;
        this._setUserCount(node.children[dir], key, count);
    }
    this._updateCount(node);
}

RedBlackTree.prototype.setUserCount = function(key, count) {
    this._setUserCount(this.root, key, count);
}

RedBlackTree.prototype.traverse = function(callback) {
    if (this.root !== null)
    {
        this._traverse(this.root, callback);
    }
}

RedBlackTree.prototype._traverse = function(node, callback) {
    if (node.children[0] !== null)
    {
        if (!this._traverse(node.children[0], callback)) return false;
    }
    if (!callback (node.key, node.value)) return false;
    if (node.children[1] !== null)
    {
        if (!this._traverse(node.children[1], callback)) return false;
    }
    return true;
}
