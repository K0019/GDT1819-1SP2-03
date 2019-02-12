#ifndef COLLIDER_H
#define COLLIDER_H

class Collider
{
public:
	enum Type // Collider types
	{
		AABB,
		OBB,
		TRIANGLE
	};

	~Collider(); // Destructor

	Type getType() const; // Get collider type
protected:
	Collider(Type type); // Constructor

private:
	Type colliderType; // Identifier of collider type
};

#endif // !COLLIDER_H
